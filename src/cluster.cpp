/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 14:54:03 by okoca             #+#    #+#             */
/*   Updated: 2024/09/08 10:26:29 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cluster.hpp"
#include <webserv.hpp>

namespace http
{
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

		add_data(server_fd, EP_SERVER, server);
		EP_EVENT event = build_event(EPOLLIN, server_fd);

		if (epoll_ctl(_instance, EPOLL_CTL_ADD, server_fd, &event) < 0)
			throw std::runtime_error("fatal: failed to add server to epoll_ctl");
	}

	void Cluster::start()
	{
		start_servers();
		while (true)
		{
			_last_ready = epoll_wait(_instance, _queue, MAX_SERVERS, 0);
			if (_last_ready < 0)
				throw std::runtime_error("fatal: epoll_wait failed!");

			handle_events();
		}
	}

	void Cluster::handle_events()
	{
		for (int i = 0; i < _last_ready; i++)
		{
			EP_EVENT	current = _queue[i];
			EP_INFO		data = _data[current.data.fd];

			if (data.type == EP_SERVER) // SERVER
			{
				handle_new_client(data.fd);
			}
			else if (data.type == EP_CLIENT)// CLIENT HANDLING HERE
			{
				if (current.events & EPOLLIN) // CLIENT REQUEST
				{
					read_client(static_cast<Client*>(data.ptr));
				}
				else if (current.events & EPOLLOUT) // CLIENT RESPONSE
				{
					write_client(static_cast<Client*>(data.ptr));
				}
			}
		}
	}

	void	Cluster::read_client(Client *client)
	{
		bool finished = client->request();

		SOCKET socket_fd = client->get_socketfd();

		if (finished)
		{
			epoll_event event = build_event(EPOLLOUT, socket_fd);

			if (epoll_ctl(_instance, EPOLL_CTL_MOD, socket_fd, &event) < 0)
				throw std::runtime_error("Failed to add to epoll instance (epoll_ctl)");
		}
	}

	void	Cluster::write_client(Client *client)
	{
		client->response();
		SOCKET socket_fd = client->get_socketfd();
		if (epoll_ctl(_instance, EPOLL_CTL_DEL, socket_fd, NULL) < 0)
			throw std::runtime_error("Failed to add to epoll instance (epoll_ctl)");
		delete client;
		_data.erase(socket_fd);
	}

	void Cluster::handle_new_client(SOCKET socket_fd)
	{
		Client	*client = new Client(socket_fd);
		SOCKET	c_socket = client->get_socketfd();

		add_data(c_socket, EP_CLIENT, client);
		EP_EVENT event = build_event(EPOLLIN, c_socket);

		if (epoll_ctl(_instance, EPOLL_CTL_ADD, c_socket, &event) < 0)
			throw std::runtime_error("fatal: failed to add client to epoll_instance!");
	}

	void Cluster::start_servers()
	{
		for (iter it = _data.begin(); it != _data.end(); it++)
		{
			if ((*it).second.type == EP_SERVER)
			{
				static_cast<Server*>((*it).second.ptr)->start_server();
			}
		}
	}

	Cluster::EP_EVENT	Cluster::build_event(uint32_t event, SOCKET fd)
	{
		EP_EVENT new_event;

		new_event.events = event;
		new_event.data.fd = fd;
		return new_event;
	}

	void Cluster::add_data(SOCKET fd, EP_TYPE type, void *ptr)
	{
		EP_INFO	ep;

		ep.fd = fd;
		ep.ptr = ptr;
		ep.type = type;
		if (_data.find(ep.fd) != _data.end())
			throw std::runtime_error("_data: fd already exists");
		_data[ep.fd] = ep;
	}

	Cluster::~Cluster()
	{
		close_instance();
		destroy_sockets();
	}

	void Cluster::destroy_sockets()
	{
		for (const_iter it = _data.begin(); it != _data.end(); it++)
		{
			EP_INFO curr = (*it).second;

			if (curr.type == EP_SERVER)
			{
				delete static_cast<Server*>(curr.ptr);
			}
			else if (curr.type == EP_CLIENT)
			{
				delete static_cast<Client*>(curr.ptr);
			}
		}
	}

	void Cluster::close_instance()
	{
		if (_instance >= 0)
			close(_instance);
	}
}
