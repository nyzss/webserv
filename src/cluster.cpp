/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 14:54:03 by okoca             #+#    #+#             */
/*   Updated: 2024/08/25 17:07:20 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

Cluster::Cluster(): _instance(-1), _last_ready(-1)
{
	_instance = epoll_create1(0);
	if (_instance < 0)
		throw std::runtime_error("fatal: failed to create epoll instance!");
}

void Cluster::add_server(const Server *serv)
{
	Server	*server = const_cast<Server*>(serv);
	SOCKET server_fd = server->get_socketfd();

	EP_EVENT event = build_event(EPOLLIN | EPOLLET, server_fd);

	if (epoll_ctl(_instance, EPOLL_CTL_ADD, server_fd, &event) < 0)
		throw std::runtime_error("fatal: failed to add server to epoll_ctl");
	_servers.push_back(server);
}

void Cluster::start()
{
	start_servers();
	while (true)
	{
		_last_ready = epoll_wait(_instance, _queue, MAX_SERVERS, -1);
		if (_last_ready < 0)
			throw std::runtime_error("fatal: epoll_wait failed!");

		handle_events();
	}
}

void Cluster::handle_events()
{
	for (int i = 0; i < _last_ready; i++)
	{
		EP_EVENT current = _queue[i];

		SOCKET socket_fd = current.data.fd;
		if (is_server(socket_fd)) // SERVER
		{
			handle_new_client(socket_fd);
		}
		else // CLIENT HANDLING HERE
		{
			if (current.data.ptr == NULL)
				throw std::runtime_error("no client ptr found in epoll_queue");
			if (current.events & EPOLLIN) // CLIENT REQUEST
			{
				read_client(static_cast<Client*>(current.data.ptr));
			}
			else if (current.events & EPOLLOUT) // CLIENT RESPONSE
			{
				write_client(static_cast<Client*>(current.data.ptr));
			}
		}
	}
}

void	Cluster::read_client(Client *client)
{
	client->request();

	SOCKET socket_fd = client->get_socketfd();
	bool finished = client->get_finished();

	epoll_event event = build_event(finished ? EPOLLOUT : EPOLLIN, client);

	if (epoll_ctl(_instance, EPOLL_CTL_MOD, socket_fd, &event) < 0)
		throw std::runtime_error("Failed to add to epoll instance (epoll_ctl)");
}

void	Cluster::write_client(Client *client)
{
	client->response();
	SOCKET socket_fd = client->get_socketfd();
	if (epoll_ctl(_instance, EPOLL_CTL_DEL, socket_fd, NULL) < 0)
		throw std::runtime_error("Failed to add to epoll instance (epoll_ctl)");
	delete client;
	_clients.erase(client);
}

void Cluster::handle_new_client(SOCKET socket_fd)
{
	Client	*client = new Client(socket_fd);
	SOCKET	c_socket = client->get_socketfd();
	_clients.insert(client);

	EP_EVENT event = build_event(EPOLLIN | EPOLLET, client);

	if (epoll_ctl(_instance, EPOLL_CTL_ADD, c_socket, &event) < 0)
		throw std::runtime_error("fatal: failed to add client to epoll_instance!");
}

bool Cluster::is_server(SOCKET fd)
{
	s_const_iter it;

	for (it = _servers.begin(); it != _servers.end(); it++)
	{
		if ((*(*it)) == fd)
			return true;
	}
	return false;
}

void Cluster::start_servers()
{
	s_iter	it;
	for (it = _servers.begin(); it != _servers.end(); it++)
		(*it)->start_server();
}

Cluster::EP_EVENT	Cluster::build_event(uint32_t event, SOCKET fd)
{
	EP_EVENT new_event;

	new_event.events = event;
	new_event.data.fd = fd;
	return new_event;
}

template <typename T>
Cluster::EP_EVENT	Cluster::build_event(uint32_t event, T *ptr)
{
	EP_EVENT new_event;

	new_event.events = event;
	new_event.data.ptr = ptr;
	return new_event;
}

Cluster::~Cluster()
{
	close_instance();
	destroy_sockets();
}

void Cluster::destroy_sockets()
{
	s_const_iter it;
	for (it = _servers.begin(); it != _servers.end(); it++)
		delete *it;

	c_const_iter c_it;
	for (c_it = _clients.begin(); c_it != _clients.end(); c_it++)
		delete *c_it;
}

void Cluster::close_instance()
{
	if (_instance >= 0)
		close(_instance);
}

// Cluster::Cluster(const Cluster &val)
// {
// 	(void)val;
// }

// Cluster & Cluster::operator=(const Cluster &val)
// {
// 	if (this != &val)
// 	{
// 		(void)val;
// 		// Copy data members from val to this object
// 	}
// 	return *this;
// }2
