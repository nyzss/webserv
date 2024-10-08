/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:48:59 by okoca             #+#    #+#             */
/*   Updated: 2024/08/28 18:26:29 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

namespace http
{
	Server::Server() : Socket()
	{
	}

	Server::Server (const std::string &ip, PORT port) : Socket()
	{
		this->_port = port;
		this->_ip = ip;
		this->_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (_fd < 0)
			throw std::runtime_error("Couldnt create socket!");

		non_blocking();
		int	opt = 1;
		if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
			throw std::runtime_error("Couldnt change socket options (address)!");
		if (setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
			throw std::runtime_error("Couldnt change socket options (port)!");

		this->_data.sin_family = AF_INET;
		this->_data.sin_port = htons(_port);
		this->_data.sin_addr.s_addr = inet_addr(_ip.c_str());
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
		if (bind(this->_fd, (sockaddr *)&this->_data, sizeof(this->_data)) < 0)
		{
			// perror("bind error");
			log_err("Couldn't bind server.", *this);
			throw std::runtime_error("bind error in start_server");
		}
		if (listen(this->_fd, MAX_CONNECTION_POOL) < 0)
		{
			// perror("listen error");
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

	void Server::connect() const
	{
		while (1)
		{
			Client client(this->_fd);
			client.get_connection();
		}
	}


	bool	Server::operator==(int val)
	{
		return _fd == val;
	}
}