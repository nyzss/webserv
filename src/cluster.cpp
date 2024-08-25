/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 14:54:03 by okoca             #+#    #+#             */
/*   Updated: 2024/08/25 15:59:04 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <stdexcept>
#include <sys/epoll.h>
#include <webserv.hpp>

Cluster::Cluster(): _instance(-1), _last_ready(-1)
{
	_instance = epoll_create1(0);
	if (_instance < 0)
		throw std::runtime_error("fatal: failed to create epoll instance!");
}

void Cluster::add_server(const Server *serv)
{
	Server	*server = const_cast<Server*>(serv);
	SOCKET server_fd = server->get_socketfd();
	// server->start_server();

	EP_EVENT event = build_event(EPOLLIN | EPOLLET, server_fd);

	if (epoll_ctl(_instance, EPOLL_CTL_ADD, server_fd, &event) < 0)
		throw std::runtime_error("fatal: failed to add server to epoll_ctl");
	_servers.push_back(server);
}

void Cluster::start()
{
	s_iter	it;
	for (it = _servers.begin(); it != _servers.end(); it++)
		(*it)->start_server();
}

Cluster::EP_EVENT	Cluster::build_event(uint32_t event, SOCKET fd)
{
	EP_EVENT new_event;

	new_event.events = event;
	new_event.data.fd = fd;
	return new_event;
}

template <typename T>
Cluster::EP_EVENT	Cluster::build_event(uint32_t event, T *ptr)
{
	EP_EVENT new_event;

	new_event.events = event;
	new_event.data.ptr = ptr;
	return new_event;
}

Cluster::~Cluster()
{
	close_instance();
	destroy_sockets();
}

void Cluster::destroy_sockets()
{
	s_const_iter it;
	for (it = _servers.begin(); it != _servers.end(); it++)
		delete *it;
}

void Cluster::close_instance()
{
	if (_instance >= 0)
		close(_instance);
}

// Cluster::Cluster(const Cluster &val)
// {
// 	(void)val;
// }

// Cluster & Cluster::operator=(const Cluster &val)
// {
// 	if (this != &val)
// 	{
// 		(void)val;
// 		// Copy data members from val to this object
// 	}
// 	return *this;
// }2
