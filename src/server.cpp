/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:48:59 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 11:46:59 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server (const std::string &ip, PORT port) : _fd(-1)
{
	this->_port = port;
	this->_ip = ip;
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);

	this->_data.sin_family = AF_INET;
	this->_data.sin_port = htons(port);
	this->_data.sin_addr.s_addr = inet_addr(ip.c_str());

}

Server::Server(const Server &val)
{
	this->_data = val._data;
	this->_fd = val._fd;
	this->_port = val._port;
	this->_ip = val._ip;
}

Server::~Server()
{
	close_server();
}

void	Server::close_server()
{
	if (this->_fd != -1)
		close(this->_fd);

	log("Closing server...", *this);
}

std::string Server::get_address() const
{
	std::ostringstream	s;

	s << this->_ip << ":" << this->_port;
	return s.str();
}