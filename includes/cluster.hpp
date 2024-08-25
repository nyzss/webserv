/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 14:53:50 by okoca             #+#    #+#             */
/*   Updated: 2024/08/25 15:58:05 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/epoll.h>
#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include <common.hpp>
#include <server.hpp>
#include <client.hpp>

# define MAX_SERVERS 16

class Cluster
{
public:
	typedef int			EP_FD;
	typedef epoll_event	EP_EVENT;
	typedef std::vector<Server*>::iterator s_iter;
	typedef std::vector<Server*>::const_iterator s_const_iter;

private:
	EP_FD		_instance;
	EP_FD		_last_ready;
	EP_EVENT	_queue[MAX_SERVERS];

private:
	std::vector<Server*> _servers;
	std::set<Client *> _clients;

private:
	// Cluster (const Cluster &val);
	// Cluster & operator=(const Cluster &val);

public:
	Cluster ();
	~Cluster ();

// UTILITY
private:
	EP_EVENT	build_event(uint32_t event, SOCKET fd);
	template <typename T> EP_EVENT	build_event(uint32_t event, T* ptr);

private:
	void add_client(const Client *serv);

public:
	void add_server(const Server *serv);
	void start();

// CLEANUP
private:
	void close_instance();
	void destroy_sockets();
};

#endif /* CLUSTER_HPP */
