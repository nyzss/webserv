/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:53:09 by okoca             #+#    #+#             */
/*   Updated: 2024/08/16 12:41:39 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

bool	exit;

void	handle_sigint(int signal)
{
	(void)signal;
	exit = true;
}

int main()
{
	signal(SIGINT, handle_sigint);
	int	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr));

	listen(server_fd, 5);

	int client_fd = accept(server_fd, NULL, NULL);

	std::string	buf_s;
	char	buf[1024] = {0};

	recv(client_fd, buf, sizeof(buf), 0);
	std::cout << "Message: " << buf << std::endl;
	close(server_fd);
}