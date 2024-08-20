/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:05:40 by okoca             #+#    #+#             */
/*   Updated: 2024/08/20 21:44:41 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

Socket::Socket () : _fd(-1), _port(-1)
{
}

Socket::Socket (const Socket &val)
{
	this->_data = val._data;
	this->_fd = val._fd;
	this->_port = val._port;
	this->_ip = val._ip;
}

Socket	&Socket::operator=(const Socket &val)
{
	if (this != &val)
	{
		this->_data = val._data;
		this->_fd = val._fd;
		this->_ip = val._ip;
		this->_port = val._port;
	}
	return *this;
}

Socket::~Socket()
{
	if (this->_fd != -1)
	{
		close(this->_fd);
		this->_fd = -1;
	}
}

void	Socket::non_blocking() const
{
	int	flags = fcntl(_fd, F_GETFL, 0);

	if (flags < 0)
		throw std::runtime_error("couldn't get flags of socketfd.");
	flags |= O_NONBLOCK;

	int	r_set = fcntl(_fd, F_SETFL, flags);

	if (r_set < 0)
		throw std::runtime_error("couldn't set flags of socketfd.");
}

SOCKET	Socket::get_socketfd() const
{
	return this->_fd;
}