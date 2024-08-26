/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:44:34 by okoca             #+#    #+#             */
/*   Updated: 2024/08/26 09:08:55 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

#include <socket.hpp>
#include <client.hpp>
#include <common.hpp>

# define MAX_CONNECTION_POOL 5

namespace http {
	class Server : public Socket
	{
	public:
		void start_server();
		void connect() const;

	public:
		Server ();
		Server (const std::string &ip, PORT port);
		Server (const Server &val);
		~Server ();
		bool	operator==(int val);

	// getters/setters
	public:
		std::string get_address() const;
	};
}

#endif /* SERVER_HPP */
