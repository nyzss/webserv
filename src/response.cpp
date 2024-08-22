/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:06:21 by okoca             #+#    #+#             */
/*   Updated: 2024/08/22 15:26:55 by okoca            ###   ########.fr       */
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

void Response::add_line(const std::string &line)
{
	_buffer += line + "\r\n";
}

void Response::add_body()
{
	_buffer += "\r\n" + _body;
}

void Response::build_body()
{
	_body = "<html><body><h1>"
		"Hello, World!</h1>";
	_body += _req.get_method_str();
	_body += "<h1>this is a custom body lmao</h1>";
	_body += _req.get_path();
	_body += "</body></html>";
}

void Response::content_type()
{
	_content_type = "Content-Type: ";
	std::string	path = _req.get_path();
	std::transform(path.begin(), path.end(), path.begin(), ::tolower);

	if (path == "/")
	{
		_content_type += "text/html";
		return ;
	}

	size_t pos = path.find_last_of('.');
	if (pos == std::string::npos)
	{
		_content_type += "application/octet-stream";
		return ;
	}
	std::string	ext = path.substr(pos + 1);
	if (ext == "html")
		_content_type += "text/html";
	else if (ext == "css")
		_content_type += "text/css";
	else if (ext == "js")
		_content_type += "application/javascript";
	else if (ext == "jpg")
		_content_type += "image/jpeg";
	else if (ext == "png")
		_content_type += "image/png";
	else if (ext == "gif")
		_content_type += "image/gif";
}

void Response::builder()
{
	add_line("HTTP/1.1 200 OK");

	content_type();
	add_line(_content_type);

	build_body();

	_content_len = "Content-Length: ";
	_content_len += to_string(_body.length());
	add_line(_content_len);
	add_line("Connection: close");
	add_body();
}

void Response::send()
{
	if (_fd < 0)
		throw std::runtime_error("response object initialised with invalid fd");
	builder();
	int r_sd = ::send(_fd, _buffer.c_str(), _buffer.length(), 0);
	if (r_sd < 0)
		throw std::runtime_error("client couldn't communicate with server!");
}

