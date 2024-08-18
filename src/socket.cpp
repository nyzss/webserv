/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:05:40 by okoca             #+#    #+#             */
/*   Updated: 2024/08/18 16:05:39 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"


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