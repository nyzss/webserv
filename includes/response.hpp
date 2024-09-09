/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:05:08 by okoca             #+#    #+#             */
/*   Updated: 2024/09/09 11:19:26 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "parser.hpp"
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <common.hpp>
#include <request.hpp>


namespace http
{
	class Response
	{
	private:
		SOCKET		_fd;
		Request		&_req;
		std::string	_buffer;

		std::string				_prefix;
		bool					_resource_exists;

		bool					_cgi;
		Parser					_message;

	public:
		Response (Request &req);
	public:
		Response (const Response &val);
		Response & operator=(const Response &val);
		~Response ();

	public:
		void send();

	private:
		void read_file(std::ifstream &file);
		void init_resource();
		void builder();
		void write();

		// void check_cgi();
		void cgi_handler(const std::string &cgi);

	private:
		void debug() const;
	};
}
#endif /* RESPONSE_HPP */
