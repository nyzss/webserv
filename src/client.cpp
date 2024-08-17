/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:47:10 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 20:55:41 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client() : Socket(), _server_sock(-1), _sock_len(0)
{
}

Client::Client(SOCKET server_sock) : Socket(), _server_sock(server_sock)
{
}

void	Client::get_connection()
{
	if (_server_sock == -1)
		throw std::runtime_error("no server socket on client");

	this->_fd = accept(_server_sock,
		(sockaddr *)&this->_data, &this->_sock_len);
	if (this->_fd < 0)
		throw std::runtime_error("accept error on client");

	char buf[1024];
	ssize_t bytes = recv(this->_fd, buf, 1023, 0);
	buf[bytes] = '\0';

	std::cout << buf << std::endl;


	std::cout << "-------CLIENT DATA--------" << std::endl;
	std::cout << "socklen_t: " << this->_sock_len << std::endl;
	std::cout << "sin_addr: " << inet_ntoa(this->_data.sin_addr) << std::endl;
	std::cout << "port: " << ntohs(this->_data.sin_port) << std::endl;
	std::cout << "family: " << this->_data.sin_family << std::endl;
	std::cout << "-----------END---------" << std::endl;
}

Client::Client(const Client &value) : Socket(value)
{
	this->_server_sock = value._server_sock;
}

Client::~Client()
{
	// log();
}
