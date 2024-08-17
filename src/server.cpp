/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:48:59 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 10:56:03 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server (std::string ip, int port)
{

}

Server::Server(const Server &val)
{
}

Server::~Server()
{

}

Server & Server::operator=(const Server &val)
{
	if (this != &val)
	{
		// Copy data members from val to this object
	}
	return *this;
}
