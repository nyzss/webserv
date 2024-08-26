/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:34:53 by okoca             #+#    #+#             */
/*   Updated: 2024/08/26 09:04:46 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <common.hpp>
#include <socket.hpp>
#include <request.hpp>
#include <response.hpp>

namespace http
{
	class Client : public Socket
	{
	private:
		Response	_res;
		Request		_req;
		SOCKET		_server_sock;
		socklen_t	_sock_len;

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
}
#endif /* CLIENT_HPP */
