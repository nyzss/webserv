/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:17:05 by okoca             #+#    #+#             */
/*   Updated: 2024/08/19 14:22:00 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

const char	*Request::methods[] = {"GET", "POST", "DELETE"};

Request::Request() : _method(GET)
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

	if (tokens.size() < 3)
		throw std::runtime_error("request received has invalid request line");
	for (int i = 0; i <= LAST; i++)
	{
		if (tokens[0] == Request::methods[i])
			this->_method = static_cast<method>(i);
	}
	this->_path = tokens[1];


	std::cout << "REQUEST METHOD: " << this->_method << ", PATH: " << this->_path << std::endl;

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
