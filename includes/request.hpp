/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:16:58 by okoca             #+#    #+#             */
/*   Updated: 2024/08/21 18:21:53y okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <common.hpp>
#include <parser.hpp>

# define DEFAULT_READ 20480

namespace http
{
	class Request
	{
	public:
		enum method
		{
			GET,
			POST,
			DELETE,
			LAST = DELETE
		};
	private:
		std::string	_buffer;

		Parser	_message;
		SOCKET	_fd;

		static const char			*_methods_arr[];
		static const std::string	separator;

		size_t	_written;
		bool	_finished;
		FD		_upload_fd;

		bool	_upload_queued;

	private:
		method		_method;
		size_t		_content_length;
		std::string	_path;

	public:
		Request ();
		Request (SOCKET sockfd);
		Request (const Request &val);
		~Request ();
		Request & operator=(const Request &val);

	public:
		bool read();
		bool write_upload();

	private:
		void receive();
		void handle_header();
		void debug() const;

		std::string find_field(const std::string &field_name);

	private:
		void handle_post();
		void handle_open_file(const char *filename);

	public:
		std::string	get_path() const;
		method get_method() const;
		std::string get_method_str() const;
		bool get_finished() const;
		SOCKET	get_sockfd() const;
		friend class Client;
	};
}

#endif /* REQUEST_HPP */
