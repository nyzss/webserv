/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:17:05 by okoca             #+#    #+#             */
/*   Updated: 2024/08/21 17:48:15 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

const char	*Request::methods_arr[] = {"GET", "POST", "DELETE"};

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
		if (tokens[0] == Request::methods_arr[i])
			this->_method = static_cast<method>(i);
	}
	this->_path = tokens[1];


	std::cout << "REQUEST METHOD: " << this->_method << ", PATH: " << this->_path << std::endl;

	std::cout << "------REST-------\n" << req << "\n";

	std::string separator = "\r\n\r\n";
	size_t header_end_pos = 0;
	// now if this condition is not satisfied it means that we have to read everything yet,
	// do some kind of checks for the followings:
	/*
	* if header has not been read fully
	* if body has not been read fully

	* or if both have been read fully
	*/
	if ((header_end_pos = req.find(separator)) != std::string::npos)
	{
		std::string	header = req.substr(0, header_end_pos);
		std::string	body = req.substr(header_end_pos + separator.length());

		std::cout << "header_len: " << header.length() << "\n";
		std::cout << "---------\n "<< header << "\n-----------\n";
		std::cout << "body_len: " << body.length() << "\n";
		std::cout << "---------\n "<< body << "\n-----------\n";
	}
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


std::string	Request::get_path() const
{
	return _path;
}
Request::method Request::get_method() const
{
	return _method;
}
std::string Request::get_method_str() const
{
	return Request::methods_arr[_method];
}