/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:06:21 by okoca             #+#    #+#             */
/*   Updated: 2024/08/22 15:47:39by okoca            ###   ########.fr       */
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
	_prefix = "example/";
	check_resource();
	send();
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
		this->_req = val._req;
		this->_buffer = val._buffer;

		this->_status_line = val._status_line;
		this->_body = val._body;
		this->_content_type = val._content_type;
		this->_content_len = val._content_len;

		this->_prefix = val._prefix;
		this->_filename = val._filename;
		this->_file.open(val._filename.c_str());
		this->_resource_exists = val._resource_exists;

		this->_raw_data = val._raw_data;
		this->_raw_size = val._raw_size;
		this->_final = val._final;
	}
	return *this;
}

Response::~Response()
{
}

void Response::end_line()
{
	_buffer += "\r\n";
}

void Response::add_line(const std::string &line)
{
	_buffer += line;
	end_line();
}

void Response::build_body()
{

	if (!_resource_exists)
		_body = HTML_NOT_FOUND;
	else
	{
		std::string	line;
		while (std::getline(_file, line))
			_body += line;
	}
}

void Response::content_type()
{
	_content_type = "Content-Type: ";
	std::string	path = _req.get_path();

	if (path == "/" || !_resource_exists)
	{
		_content_type += "text/html";
		return ;
	}

	size_t pos = path.find_last_of('.');
	if (pos == std::string::npos)
	{
		_content_type += "text/html";
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

void	Response::status_line()
{
	_status_line = "HTTP/1.1 ";
	if (!_resource_exists)
		_status_line += STATUS.at(NOT_FOUND);
	else
		_status_line += STATUS.at(OK);
}

void Response::builder()
{
	status_line();
	add_line(_status_line);

	// build_body();
	read_file();

	content_type();
	add_line(_content_type);

	_content_len = "Content-Length: ";
	_content_len += to_string(_raw_size);
	add_line(_content_len);
	add_line("Connection: close");
	end_line();

	_final.reserve(_buffer.size() + _raw_size);

	_final.insert(_final.end(), _buffer.begin(), _buffer.end());
	_final.insert(_final.end(), _raw_data.begin(), _raw_data.end());

	std::string	partial(_raw_data.begin(), _raw_data.end());
	std::cout << "---------------\nRESPONSE HEADER: \n" << _buffer << "\n";
	if (_raw_size > 100)
		std::cout << "---------------\nRESPONSE BODY: \n" << "[OMITTED]" << std::endl;
	else
		std::cout << "---------------\nRESPONSE BODY: \n" << partial << std::endl;
}

void Response::send()
{
	if (_fd < 0)
		throw std::runtime_error("response object initialised with invalid fd");
	builder();
	std::cout << "----------- SIZE: " << _final.size() << std::endl;
	int r_sd = ::send(_fd, _final.data(), _final.size(), 0);
	if (r_sd < 0)
		throw std::runtime_error("client couldn't communicate with server!");
}

void Response::read_file()
{
	if (!_resource_exists)
	{
		_raw_data.insert(_raw_data.end(), HTML_NOT_FOUND.begin(), HTML_NOT_FOUND.end());
		_raw_size = _raw_data.size();
		return ;
	}
	_raw_size = _file.tellg();
	_file.seekg(0, std::ios::beg);
	_raw_data.resize(_raw_size);
	std::cout << "_raw_size: " << _raw_size << std::endl;

	if (!_file.read(reinterpret_cast<char *>(_raw_data.data()), _raw_size))
		throw std::runtime_error("failed to read file!");
}

void Response::check_resource()
{
	_filename = _prefix;

	if (_req.get_path() == "/")
		_filename += "index.html";
	else
		_filename += _req.get_path();

	_file.open(_filename.c_str(), std::ios::binary | std::ios::ate);
	_resource_exists = _file.good();
	if (!_resource_exists)
		return ;
}