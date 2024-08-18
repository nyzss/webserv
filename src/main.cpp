/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:53:09 by okoca             #+#    #+#             */
/*   Updated: 2024/08/18 17:22:04 by okoca            ###   ########.fr       */
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

		std::vector<epoll_event>	e_servers;
		// std::vector<epoll_event>	e_queue;
		epoll_event	e_queue[MAX_SERVERS];

		epoll_instance = epoll_create1(0);
		if (epoll_instance < 0)
			throw std::runtime_error("Failed to create epoll instance");


		std::vector<Server*>::iterator	it;
		for (it = servers.begin(); it != servers.end(); it++)
		{
			SOCKET	socket_fd = (*(*it)).get_socket_fd();
			(*(*it)).start_server();
			epoll_event	event;
			event.events = EPOLLIN;
			event.data.fd = socket_fd;

			e_servers.push_back(event);
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
				std::cout << "ready: " << e_queue[i].events << ", for: " << e_queue[i].data.fd << std::endl;
				Client	client(e_queue[i].data.fd);
				client.get_connection();
			}
		}

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	std::vector<Server*>::iterator it;
	for (it = servers.begin(); it != servers.end(); it++)
		delete (*it);
	if (epoll_instance != -1)
		close(epoll_instance);
}

// int main()
// {
// 	std::vector<Server*> servers;
// 	try
// 	{
// 		signal(SIGINT, handle_sigint);

// 		servers.push_back(new Server("0.0.0.0", 8080));
// 		servers.push_back(new Server("0.0.0.0", 8081));
// 		servers.push_back(new Server("0.0.0.0", 8082));

// 		std::vector<Server*>::iterator it;
// 		for (it = servers.begin(); it != servers.end(); it++)
// 		{
// 			std::cout << "ip: " << (*(*it)).get_address() << std::endl;
// 			std::cout << "fd: " << (*(*it)).get_socket_fd() << std::endl;
// 		}
// 		signal(SIGINT, SIG_DFL);
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << '\n';
// 	}
// 	std::vector<Server*>::iterator it;
// 	for (it = servers.begin(); it != servers.end(); it++)
// 		delete (*it);
// }
