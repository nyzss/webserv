/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:05:08 by okoca             #+#    #+#             */
/*   Updated: 2024/08/25 20:49:02 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <common.hpp>
#include <request.hpp>

class Response
{
private:
	SOCKET		_fd;
	Request		&_req;
	std::string	_buffer;

	std::string				_prefix;
	std::vector<uint8_t>	_raw_data;
	std::streamsize			_raw_size;
	std::vector<uint8_t>	_final;
	bool					_resource_exists;

	bool					_cgi;

public:
	Response (Request &req);
public:
	Response (const Response &val);
	Response & operator=(const Response &val);
	~Response ();

public:
	void send();

private:
	void add_line(const std::string &line);
	void read_file(std::ifstream &file);
	void end_line();
	void init_resource();
	void builder();
	void write();
	void combine();

	void check_cgi();
	void cgi_handler(const std::string &cgi);

private:
	std::string	init_status_line() const;
	std::string init_content_type() const;
	std::string	init_content_len() const;
	std::string	init_connection() const;

	void debug() const;
};

#endif /* RESPONSE_HPP */
