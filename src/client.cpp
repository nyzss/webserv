/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:47:10 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 20:24:05 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client() : Socket()
{
}

Client::Client(SOCKET server_sock) : Socket(), _server_sock(server_sock)
{
}

Client::Client(const Client &value) : Socket(value)
{
	this->_server_sock = value._server_sock;
}

Client::~Client()
{
	// log();
}
