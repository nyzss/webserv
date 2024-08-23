/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:05:08 by okoca             #+#    #+#             */
/*   Updated: 2024/08/23 09:57:53 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <common.hpp>
#include <request.hpp>


enum STATUS_CODE
{
	OK,
	NOT_FOUND,
};

class Response
{
private:
	static const std::string	HTML_NOT_FOUND;
	static const std::map<STATUS_CODE, const char *> STATUS;

private:
	SOCKET		_fd;
	Request		_req;
	std::string	_buffer;

	std::string	_status_line;
	std::string	_body;
	std::string	_content_type;
	std::string	_content_len;

	std::string		_prefix;
	std::ifstream	_file;
	std::string		_filename;
	bool			_resource_exists;

public:
	Response ();
	Response (const Response &val);
	~Response ();
	Response & operator=(const Response &val);

public:
	void builder();
	void send();
	void add_line(const std::string &line);
	void add_body();
	void content_type();
	void status_line();
	void build_body();
	void check_resource();

public:
	Response (const Request &req);
};

#endif /* RESPONSE_HPP */
