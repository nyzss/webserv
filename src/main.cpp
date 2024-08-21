/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:53:09 by okoca             #+#    #+#             */
/*   Updated: 2024/08/21 22:14:08 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

void	handle_sigint(int sig)
{
	(void)sig;
	throw std::runtime_error("exiting...");
}

# define MAX_SERVERS 16


int main()
{
	std::vector<Server*> servers;
	std::set<Client *> e_clients;
	int	epoll_instance = -1;
	try
	{
		signal(SIGINT, handle_sigint);

		int	event_ready = 0;
		servers.push_back(new Server("0.0.0.0", 8080));
		servers.push_back(new Server("0.0.0.0", 8081));
		servers.push_back(new Server("0.0.0.0", 8082));
		servers.push_back(new Server("0.0.0.0", 8083));
		servers.push_back(new Server("0.0.0.0", 8084));

		// std::vector<epoll_event>	e_queue;
		epoll_event	e_queue[MAX_SERVERS];

		epoll_instance = epoll_create1(0);
		if (epoll_instance < 0)
			throw std::runtime_error("Failed to create epoll instance");


		std::vector<Server*>::iterator	it;
		for (it = servers.begin(); it != servers.end(); it++)
		{
			SOCKET	socket_fd = (*(*it)).get_socketfd();
			(*(*it)).start_server();
			epoll_event	event;
			event.events = EPOLLIN | EPOLLET;
			event.data.fd = socket_fd;

			int r_epollctl = epoll_ctl(epoll_instance, EPOLL_CTL_ADD, socket_fd, &event);
			if (r_epollctl < 0)
			{
				perror("epoll ctl");
				throw std::runtime_error("Failed to add to epoll instance (epoll_ctl)");
			}
		}

		while (true)
		{
			event_ready = epoll_wait(epoll_instance, e_queue, MAX_SERVERS, -1);
			if (event_ready < 0)
				throw std::runtime_error("Failed on epoll_wait");

			for (int i = 0; i < event_ready; i++)
			{
				std::vector<Server *>::const_iterator it;

				for (it = servers.begin(); it != servers.end(); it++)
				{
					if ((*(*it)) == e_queue[i].data.fd)
						break;
				}
				if (it != servers.end())
				{
					Client	*client = new Client(e_queue[i].data.fd);
	 				e_clients.insert(client);

					epoll_event	c_event;
					c_event.events = EPOLLIN | EPOLLET;
					c_event.data.ptr = static_cast<void *>(client);

					int r_epoll = epoll_ctl(epoll_instance, EPOLL_CTL_ADD, client->get_socketfd(), &c_event);
					if (r_epoll < 0)
					{
						perror("epoll ctl");
						throw std::runtime_error("Failed to add to epoll instance (epoll_ctl)");
					}
				}
				else
				{
					if (e_queue[i].events & EPOLLIN)
					{
						Client	*client;

						if (e_queue[i].data.ptr == NULL)
							throw std::runtime_error("no client ptr found in epoll_queue");
						client = static_cast<Client *>(e_queue[i].data.ptr);

						client->request();

						epoll_event c_event = e_queue[i];
						if (client->get_finished() == false)
							c_event.events = EPOLLIN;
						else
							c_event.events = EPOLLOUT;

						int r_epoll = epoll_ctl(epoll_instance, EPOLL_CTL_MOD, client->get_socketfd(), &c_event);
						if (r_epoll < 0)
						{
							perror("epoll ctl");
							throw std::runtime_error("Failed to add to epoll instance (epoll_ctl)");
						}
					}
					else if (e_queue[i].events & EPOLLOUT)
					{
						Client	*client;

						if (e_queue[i].data.ptr == NULL)
							throw std::runtime_error("no client ptr found in epoll_queue");
						client = static_cast<Client *>(e_queue[i].data.ptr);

						client->response();
						int r_epoll = epoll_ctl(epoll_instance, EPOLL_CTL_DEL, client->get_socketfd(), &(e_queue[i]));
						if (r_epoll < 0)
						{
							perror("epoll ctl");
							throw std::runtime_error("Failed to add to epoll instance (epoll_ctl)");
						}
						delete client;
						e_clients.erase(client);
					}
				}
			}
		}

	}
	catch(const std::exception& e)
	{
		log_err(e.what());
	}

	std::vector<Server*>::iterator it;
	for (it = servers.begin(); it != servers.end(); it++)
		delete (*it);

	std::set<Client *>::const_iterator eit;
	for (eit = e_clients.begin(); eit != e_clients.end(); eit++)
		delete (*eit);

	if (epoll_instance != -1)
		close(epoll_instance);
}