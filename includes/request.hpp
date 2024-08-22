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

# define DEFAULT_READ 1024
# define CONTENT_TYPE "Content-Type: "
# define CONTENT_LENGTH "Content-Length: "

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
	std::string	_header;
	std::string	_body;
	SOCKET	_fd;
	static const char *_methods_arr[];
	static const std::string	separator;
	size_t	_needed_size;
	size_t	_current_size;
	bool	_finished;
	bool	_header_finished;

private:
	method		_method;
	size_t		_content_length;
	std::string	_path;
	std::string	_content_type;

public:
	Request ();
	Request (SOCKET sockfd);
	Request (const Request &val);
	~Request ();
	Request & operator=(const Request &val);
	// Request (const std::string &req); // obsolete

public:
	void read();
	void receive();
	void check_buffer();

	void handle_header();
	void handle_body();

	std::string find_field(const std::string &field_name);

public:
	std::string	get_path() const;
	method get_method() const;
	std::string get_method_str() const;
	bool get_finished() const;
	SOCKET	get_sockfd() const;
};

#endif /* REQUEST_HPP */
