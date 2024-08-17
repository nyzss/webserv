/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:53:09 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 11:21:44 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

int main()
{
	Server	serv("0.0.0.0", 8080);
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