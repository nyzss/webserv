/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:44:34 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 20:17:05 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

#include <socket.hpp>
#include <webserv.hpp>

# define MAX_CONNECTION_POOL 5

class Server : public Socket
{
private:
	Server ();

public:
	void start_server();
	void close_server();

	// void accept_connection();

public:
	Server (const std::string &ip, PORT port);
	Server (const Server &val);
	~Server ();

// getters/setters
public:
	std::string get_address() const;

};

#endif /* SERVER_HPP */
