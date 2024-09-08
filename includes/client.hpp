/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:34:53 by okoca             #+#    #+#             */
/*   Updated: 2024/09/08 15:04:58 by okoca            ###   ########.fr       */
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
		Request		_req;
		Response	_res;
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
		bool	request();
		bool	upload();
		void	response();
		void	get_connection();

	public:
		bool	get_upload_status() const;
		FD		get_upload_fd() const;

	private:
		void	debug() const;

	};
}
#endif /* CLIENT_HPP */
