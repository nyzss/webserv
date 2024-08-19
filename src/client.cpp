/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:47:10 by okoca             #+#    #+#             */
/*   Updated: 2024/08/19 09:09:04 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include <webserv.hpp>

Client::Client() : Socket(), _server_sock(-1), _sock_len(sizeof(_data))
{
	std::memset(&this->_data, 0, sizeof(this->_data));
}

Client::Client(SOCKET server_sock) : Socket(), _server_sock(server_sock), _sock_len(sizeof(_data))
{
}

void	Client::get_connection()
{
	if (_server_sock == -1)
		throw std::runtime_error("no server socket on client");

	this->_fd = accept(_server_sock,
		(sockaddr *)&this->_data, &this->_sock_len);

	// this->_fd = accept(_server_sock, NULL, NULL);
	if (this->_fd < 0)
		throw std::runtime_error("accept error on client");

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
	this->_sock_len = value._sock_len;
}

Client::~Client()
{}

void	Client::request() const
{
	std::string	buffer;

	if (this->_fd < 0)
		throw std::runtime_error("trying to read request with no fd");
	char _buf[1024];
	ssize_t bytes = recv(this->_fd, _buf, 1023, 0);
	if (bytes < 0)
		throw std::runtime_error("failed to read request: recv error");
	_buf[bytes] = '\0';
	buffer += _buf;

	std::string	first_line;
	size_t pos = 0;
	if ((pos = buffer.find('\n')) == std::string::npos)
		throw std::runtime_error("no line found in request");
	first_line = buffer.substr(0, pos);

	std::vector<std::string> tokens = ws_split(first_line, ' ');
	std::vector<std::string>::const_iterator it;

	for (it = tokens.begin(); it != tokens.end(); it++)
	{
		std::cout << *it << std::endl;
	}

	std::cout << "------REST-------\n" << buffer << "\n";
}