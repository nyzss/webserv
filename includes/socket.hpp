/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:36:56 by okoca             #+#    #+#             */
/*   Updated: 2024/08/26 09:09:08 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <common.hpp>

namespace http
{
	class Socket
	{
	protected:
		SOCKET		_fd;
		PORT		_port;
		sockaddr_in	_data;
		std::string	_ip;
	public:
		Socket ();
		Socket	&operator=(const Socket &val);
		Socket (const Socket &val);
		virtual ~Socket () = 0;

	public:
		void	non_blocking() const;
		SOCKET	get_socketfd() const;
	};
}

#endif /* SOCKET_HPP */