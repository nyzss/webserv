/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:44:34 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 12:01:09 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

#include <webserv.hpp>

# define MAX_CONNECTION_POOL 5

class Server
{
public:
	typedef int	SOCKET;
	typedef int	PORT;

private:
	SOCKET		_fd;
	PORT		_port;
	sockaddr_in	_data;
	std::string	_ip;

private:
	Server () : _fd(-1) {}

public:
	void start_server();
	void close_server();

public:
	Server (const std::string &ip, PORT port);
	Server (const Server &val);
	~Server ();

// getters/setters
public:
	std::string get_address() const;

};

#endif /* SERVER_HPP */
