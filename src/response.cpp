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
	_prefix = "example/";
	this->_fd = req.get_sockfd();
	if (_fd < 0)
		throw std::runtime_error("response object initialised with invalid fd");
	this->_req = req;
	init_resource();
	builder();
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
		this->_resource_exists = val._resource_exists;

		this->_raw_data = val._raw_data;
		this->_raw_size = val._raw_size;
		this->_final = val._final;
	}
	return *this;
}

Response::~Response()
{}

void Response::end_line()
{
	_buffer += "\r\n";
}

void Response::add_line(const std::string &line)
{
	_buffer += line;
	end_line();
}

std::string Response::init_content_type() const
{
	std::string content_type = "Content-Type: ";
	std::string	path = _req.get_path();

	size_t pos = path.find_last_of('.');
	if (path == "/" || !_resource_exists || pos == std::string::npos)
	{
		content_type += "text/html";
		return content_type;
	}

	std::string	ext = path.substr(pos + 1);
	if (ext == "html")
		content_type += "text/html";
	else if (ext == "css")
		content_type += "text/css";
	else if (ext == "js")
		content_type += "application/javascript";
	else if (ext == "jpg")
		content_type += "image/jpeg";
	else if (ext == "png")
		content_type += "image/png";
	else if (ext == "gif")
		content_type += "image/gif";
	return content_type;
}

std::string	Response::init_status_line() const
{
	std::string status_line = Defaults::Fields()[VERSION];
	if (!_resource_exists)
		status_line += Defaults::Status()[NOT_FOUND];
	else
		status_line += Defaults::Status()[OK];
	return status_line;
}

std::string	Response::init_content_len() const
{
	std::string content_len = Defaults::Fields()[CONTENT_LENGTH];
	content_len += to_string(_raw_size);
	return content_len;
}

std::string	Response::init_connection() const
{
	std::string connection = Defaults::Fields()[CONNECTION];
	connection += "close";
	return connection;
}

void Response::builder()
{
	add_line(init_status_line());
	add_line(init_content_type());
	add_line(init_content_len());
	add_line(init_connection());

	end_line();

	_final.reserve(_buffer.size() + _raw_size);

	_final.insert(_final.end(), _buffer.begin(), _buffer.end());
	_final.insert(_final.end(), _raw_data.begin(), _raw_data.end());

	// debug();
}

void Response::send()
{
	int r_sd = ::send(_fd, _final.data(), _final.size(), 0);
	if (r_sd < 0)
		throw std::runtime_error("client couldn't communicate with server!");
}

void Response::read_file(std::ifstream &file)
{
	if (!_resource_exists)
	{
		_raw_data.insert(_raw_data.end(), Defaults::html_not_found.begin(), Defaults::html_not_found.end());
		_raw_size = _raw_data.size();
		return ;
	}
	_raw_size = file.tellg();
	file.seekg(0, std::ios::beg);
	_raw_data.resize(_raw_size);

	if (!file.read(reinterpret_cast<char *>(_raw_data.data()), _raw_size))
		throw std::runtime_error("failed to read file!");
}

void Response::init_resource()
{
	std::string filename = _prefix;
	std::ifstream	file;

	if (_req.get_path() == "/")
		filename += "index.html";
	else
		filename += _req.get_path();

	file.open(filename.c_str(), std::ios::binary | std::ios::ate);
	_resource_exists = file.good();
	if (!_resource_exists)
		return ;
	read_file(file);
}

void Response::debug()
{
	std::string	partial(_raw_data.begin(), _raw_data.end());
	std::cout << "---------------\nRESPONSE HEADER: \n" << _buffer << "\n";

	std::cout << "---------------\nRESPONSE BODY: \n";
	if (_raw_size > 100)
		std::cout << "[OMITTED]" << std::endl;
	else
		std::cout << partial << std::endl;
}