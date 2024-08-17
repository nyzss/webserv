/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:36:56 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 19:55:05 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <webserv.hpp>

class Socket
{
public:
	typedef int	SOCKET;
	typedef int	PORT;

protected:
	SOCKET		_fd;
	PORT		_port;
	sockaddr_in	_data;
	std::string	_ip;

protected:
	Socket () : _fd(-1), _port(-1) {}
	Socket (const Socket &val)
	{
		this->_data = val._data;
		this->_fd = val._fd;
		this->_port = val._port;
		this->_ip = val._ip;
	}
public:
	virtual ~Socket () = 0;
};

#endif /* SOCKET_HPP */
