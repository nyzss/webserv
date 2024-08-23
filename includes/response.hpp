/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:05:08 by okoca             #+#    #+#             */
/*   Updated: 2024/08/23 15:21:31 by okoca            ###   ########.fr       */
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
	Request		_req;
	std::string	_buffer;

	std::string	_status_line;
	std::string	_body;
	std::string	_content_type;
	std::string	_content_len;

	std::string				_prefix;
	std::vector<uint8_t>	_raw_data;
	std::streampos			_raw_size;
	std::vector<uint8_t>	_final;
	bool					_resource_exists;

public:
	Response ();
	Response (const Response &val);
	~Response ();
	Response & operator=(const Response &val);

private:
	void builder();
	void send();
	void add_line(const std::string &line);
	void content_type();
	void init_resource();
	void read_file(std::ifstream &file);
	void end_line();
	void debug();

private:
	std::string	init_status_line() const;
	std::string init_content_type() const;
	std::string	init_content_len() const;
	std::string	init_connection() const;

public:
	Response (const Request &req);
};

#endif /* RESPONSE_HPP */
