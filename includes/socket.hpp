/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:36:56 by okoca             #+#    #+#             */
/*   Updated: 2024/08/18 16:05:35 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <common.hpp>

class Socket
{
protected:
	SOCKET		_fd;
	PORT		_port;
	sockaddr_in	_data;
	std::string	_ip;
public:
	Socket ();
	Socket	&operator=(const Socket &val);
	Socket (const Socket &val);
	virtual ~Socket () = 0;
};

#endif /* SOCKET_HPP */