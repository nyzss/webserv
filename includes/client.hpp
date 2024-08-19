/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:34:53 by okoca             #+#    #+#             */
/*   Updated: 2024/08/19 08:39:34 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <common.hpp>
#include <socket.hpp>

class Client : public Socket
{
private:
	SOCKET		_server_sock;
	socklen_t	_sock_len;

private:
	Client ();

public:
	Client (SOCKET server_sock);
	Client (const Client &val);
	~Client ();

public:
	void	get_connection();
	void	request() const;
};

#endif /* CLIENT_HPP */
