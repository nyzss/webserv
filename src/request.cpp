/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:17:05 by okoca             #+#    #+#             */
/*   Updated: 2024/08/19 09:31:56 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

Request::Request() : _method(NONE)
{
}

Request::Request(const std::string &req)
{
	std::string	first_line;
	size_t pos = 0;
	if ((pos = req.find('\n')) == std::string::npos)
		throw std::runtime_error("no line found in request");
	first_line = req.substr(0, pos);

	std::vector<std::string> tokens = ws_split(first_line, ' ');
	std::vector<std::string>::const_iterator it;

	for (it = tokens.begin(); it != tokens.end(); it++)
	{
		std::cout << "REQ_LINE: " << *it << std::endl;
	}

	std::cout << "------REST-------\n" << req << "\n";
}

Request::Request(const Request &val)
{
	this->_method = val._method;
	this->_path = val._path;
}

Request::~Request()
{
}

Request & Request::operator=(const Request &val)
{
	if (this != &val)
	{
		this->_method = val._method;
		this->_path = val._path;
	}
	return *this;
}
