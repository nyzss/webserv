/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:53:09 by okoca             #+#    #+#             */
/*   Updated: 2024/08/16 15:41:31 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

int	port = 8080;

int	server_fd = -1;
int client_fd = -1;

bool	end_sig = false;

void	handle_sigint(int sig)
{
	(void)sig;
	end_sig = true;
	if (server_fd != -1)
		close(server_fd);
}

int main()
{
	signal(SIGINT, handle_sigint);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr));

	std::cout << "[" << server_fd << "]" << " listening on port: " << port << std::endl;
	listen(server_fd, 5);

	while (1)
	{
		client_fd = accept(server_fd, NULL, NULL);
		if (end_sig)
			break;
		std::cout << "received signal by connection: [" << client_fd << "]" << std::endl;

		std::string	buf_s;
		char	buf[1024] = {0};

		recv(client_fd, buf, sizeof(buf), 0);
		std::cout << "Message: " << std::endl << buf << std::endl;
	}
	if (server_fd != -1)
		close(server_fd);
}