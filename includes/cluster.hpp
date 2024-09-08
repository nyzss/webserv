/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 14:53:50 by okoca             #+#    #+#             */
/*   Updated: 2024/09/08 14:06:55 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include <common.hpp>
#include <server.hpp>
#include <client.hpp>

# define MAX_SERVERS 64

namespace http
{
	class Cluster
	{
	// EPOLL FD MANAGEMENT HELPERS
	private:
		enum	EP_TYPE
		{
			EP_SERVER,
			EP_CLIENT,
			EP_RESOURCE,
			EP_CGI
		};

		struct	EP_INFO
		{
			SOCKET	fd;
			EP_TYPE	type;
			void*	ptr;
		};

	// MAIN TYPEDEFS
	private:
		typedef int			EP_FD;
		typedef epoll_event	EP_EVENT;

		typedef std::map<SOCKET, EP_INFO>::iterator			iter;
		typedef std::map<SOCKET, EP_INFO>::const_iterator	const_iter;

	private:
		EP_FD		_instance;
		EP_FD		_last_ready;
		EP_EVENT	_queue[MAX_SERVERS];

	// UNDERLYING CONTAINER
	private:
		std::map<SOCKET, EP_INFO>	_data;

	public:
		Cluster ();
		~Cluster ();

	// UTILITY
	private:
		EP_EVENT	build_event(uint32_t event, SOCKET fd);

		void add_data(SOCKET fd, EP_TYPE type, void *ptr);

		void ctl(int op, SOCKET fd, EP_EVENT* event);

	private:
		void start_servers();
		void handle_events();

		void add_client(const Client *serv);
		void handle_new_client(SOCKET socket_fd);
		void read_client(Client *client);
		void write_client(Client *client);

	public:
		void add_server(const Server *serv);
		void start();

	// CLEANUP
	private:
		void close_instance();
		void destroy_sockets();
	};
}
#endif /* CLUSTER_HPP */
