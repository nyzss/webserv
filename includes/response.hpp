/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:05:08 by okoca             #+#    #+#             */
/*   Updated: 2024/08/22 15:05:56 by okoca            ###   ########.fr       */
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

	std::string	_body;
	std::string	_content_type;
	std::string	_content_len;

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
	void build_body();

public:
	Response (const Request &req);
};

#endif /* RESPONSE_HPP */
