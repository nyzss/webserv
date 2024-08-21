/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:34:53 by okoca             #+#    #+#             */
/*   Updated: 2024/08/21 18:11:25 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <common.hpp>
#include <socket.hpp>
#include <request.hpp>

class Client : public Socket
{
private:
	SOCKET		_server_sock;
	socklen_t	_sock_len;
	Request		_req;

private:
	Client ();

public:
	Client (SOCKET server_sock);
	Client (const Client &val);
	~Client ();

public:
	void	reset();
	void	request();
	void	response();
	void	get_connection();

public:
	bool	get_finished() const;
};

#endif /* CLIENT_HPP */
