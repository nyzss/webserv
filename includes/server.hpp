/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:44:34 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 10:57:09 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>


class Server
{
public:
	typedef int	SOCKET;
	typedef int	PORT;

private:
	SOCKET		_fd;
	PORT		_port;
	sockaddr_in	_data;

private:
	Server (std::string ip, PORT port);
	Server (const Server &val);
	Server & operator=(const Server &val);
public:
	~Server ();
};

#endif /* SERVER_HPP */
