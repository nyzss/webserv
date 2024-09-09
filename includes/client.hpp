/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:34:53 by okoca             #+#    #+#             */
/*   Updated: 2024/09/09 12:00:17 by okoca            ###   ########.fr       */
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
		bool		_cgi;
		std::string	_cgi_buffer;
		PIPE		_pipe;

	private:
		Client ();

	public:
		Client (SOCKET server_sock);
		Client (const Client &val);
		~Client ();

	public:
		void	reset();
		bool	request();
		void	response();
		bool	cgi();
		void	get_connection();

	private:
		void	cgi_handler(const std::string &cgi);
		void	debug() const;

	public:
		PIPE	get_pipe_fd() const;
		bool	has_cgi() const;
	};
}
#endif /* CLIENT_HPP */
