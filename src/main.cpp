/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:53:09 by okoca             #+#    #+#             */
/*   Updated: 2024/08/18 15:23:19 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

void	handle_sigint(int sig)
{
	(void)sig;
	throw std::runtime_error("exiting...");
}

# define MAX_EPOLL_EVENTS 16


// Server	serv("0.0.0.0", 8080);

// serv.start_server();
// serv.connect();
int main()
{
	try
	{
		signal(SIGINT, handle_sigint);

		int	epoll_instance = -1;
		int	event_ready = 0;
		std::vector<Server> servers;
		servers.push_back(Server("0.0.0.0", 8080));
		servers.push_back(Server("0.0.0.0", 8081));

		std::vector<epoll_event>	e_servers;
		// std::vector<epoll_event>	e_queue;
		epoll_event	e_queue[MAX_EPOLL_EVENTS];

		epoll_instance = epoll_create1(0);
		if (epoll_instance < 0)
			throw std::runtime_error("Failed to create epoll instance");


		std::vector<Server>::iterator	it;
		for (it = servers.begin(); it != servers.end(); it++)
		{
			SOCKET	socket_fd = (*it).get_socket_fd();
			(*it).start_server();
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
			event_ready = epoll_wait(epoll_instance, e_queue, MAX_EPOLL_EVENTS, -1);
			if (event_ready < 0)
				throw std::runtime_error("Failed on epoll_wait");

			for (int i = 0; i < event_ready; i++)
			{
				std::cout << "ready: " << e_queue[i].events << ", for: " << e_queue[i].data.fd << std::endl;
			}
		}

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

// # define BUF_SIZE 1024

// int	port = 8080;

// int	server_fd = -1;
// int client_fd = -1;

// bool	end_sig = false;

// void	handle_sigint(int sig)
// {
// 	(void)sig;
// 	throw std::runtime_error("exiting...");
// 	// end_sig = true;
// 	// if (server_fd != -1)
// 	// 	close(server_fd);
// }

// int main()
// {
// 	signal(SIGINT, handle_sigint);
// 	server_fd = socket(AF_INET, SOCK_STREAM, 0);

// 	sockaddr_in server_addr;
// 	server_addr.sin_family = AF_INET;
// 	server_addr.sin_port = htons(port);
// 	server_addr.sin_addr.s_addr = INADDR_ANY;

// 	bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr));

// 	std::cout << "[" << server_fd << "]" << " listening on port: " << port << std::endl;
// 	listen(server_fd, 5);

// 	try
// 	{
// 		while (!end_sig)
// 		{
// 			client_fd = accept(server_fd, NULL, NULL);
// 			std::cout << "received signal by connection: [" << client_fd << "]" << std::endl;

// 			std::string	buf_s;
// 			ssize_t total_bytes = 0;
// 			// while (1)
// 			// {
// 				char	buf[BUF_SIZE] = {0};

// 				ssize_t b_read = recv(client_fd, buf, BUF_SIZE - 1, 0);
// 				if (b_read <= 0)
// 					break;
// 				buf[b_read] = '\0';
// 				buf_s += buf;
// 				total_bytes += b_read;
// 			// }
// 			std::cout << "CONTENT: " << std::endl << buf_s << std::endl;
// 			std::cout << "bytes read: " << total_bytes << std::endl;
// 			char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContentLength: 12\n\nHello world!";
// 			send(client_fd, hello, strlen(hello), 0);
// 			close(client_fd);
// 		}
// 	}

// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << '\n';
// 	}
// 	if (server_fd != -1)
// 		close(server_fd);
// }