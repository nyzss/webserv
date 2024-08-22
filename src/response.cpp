/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:06:21 by okoca             #+#    #+#             */
/*   Updated: 2024/08/22 14:27:18 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

Response::Response() : _fd(-1)
{
}

Response::Response(const Request &req)
{
	this->_fd = req.get_sockfd();
	if (_fd < 0)
		throw std::runtime_error("response object initialised with invalid fd");
	this->_req = req;
}

Response::Response(const Response &val)
{
	*this = val;
}

Response & Response::operator=(const Response &val)
{
	if (this != &val)
	{
		this->_fd = val._fd;
	}
	return *this;
}

Response::~Response()
{
}

void Response::send()
{
	if (_fd < 0)
		throw std::runtime_error("response object initialised with invalid fd");

	std::string	buffer;
	buffer =
		"HTTP/1.1 200 OK\n"
		"Content-Type: text/html\n"
		"Content-Length: 44\n"
		"Connection: close\n\n"
		"<html><body><h1>"
		"Hello, World!</h1>";
	buffer += _req.get_method_str();
	buffer += "</body></html>";
	int r_sd = ::send(_fd, buffer.c_str(), buffer.length(), 0);
	if (r_sd < 0)
		throw std::runtime_error("client couldn't communicate with server!");
}

