/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:48:59 by okoca             #+#    #+#             */
/*   Updated: 2024/08/18 14:54:14 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() : Socket()
{
}

Server::Server (const std::string &ip, PORT port) : Socket()
{
	this->_port = port;
	this->_ip = ip;
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);

	this->_data.sin_family = AF_INET;
	this->_data.sin_port = htons(port);
	this->_data.sin_addr.s_addr = inet_addr(ip.c_str());
}

Server::Server(const Server &val) : Socket(val)
{
}

Server::~Server()
{
	log("Closing server...", *this);
}

void	Server::start_server()
{
	log("Server is starting..", *this);
	int r_bind = bind(this->_fd, (sockaddr *)&this->_data, sizeof(this->_data));
	if (r_bind < 0)
	{
		log_err("Couldn't bind server.", *this);
		throw std::runtime_error("bind error in start_server");
	}

	int r_listen = listen(this->_fd, MAX_CONNECTION_POOL);
	if (r_listen < 0)
	{
		log_err("Couldn't listen on server address/port.", *this);
		throw std::runtime_error("listen error in start_server");
	}
	log("Server started! Currently listening for requests..", *this);
}

std::string Server::get_address() const
{
	std::ostringstream	s;

	s << this->_ip << ":" << this->_port;
	return s.str();
}

SOCKET	Server::get_socket_fd() const
{
	return this->_fd;
}

void Server::connect() const
{
	while (1)
	{
		Client client(this->_fd);
		client.get_connection();
	}
}
