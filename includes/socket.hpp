/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:36:56 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 20:14:21 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <webserv.hpp>

class Socket
{
protected:
	SOCKET		_fd;
	PORT		_port;
	sockaddr_in	_data;
	std::string	_ip;

protected:
	Socket ();
	Socket (const Socket &val);
public:
	virtual ~Socket () = 0;
};

#endif /* SOCKET_HPP */
