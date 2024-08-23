/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:17:05 by okoca             #+#    #+#             */
/*   Updated: 2024/08/23 14:50:02 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

const char	*Request::_methods_arr[] = {"GET", "POST", "DELETE"};
const std::string Request::separator = "\r\n\r\n";

Request::Request()
{
	_method = GET;
	_needed_size = 0;
	_current_size = 0;
	_finished = false;
	_header_finished = false;
	_content_length = 0;
	_fd = -1;
}

Request::Request(SOCKET sockfd)
{
	_fd = sockfd;
	_finished = false;
	_header_finished = false;
	_current_size = 0;
	_needed_size = 0;
	_content_length = (0);
}

Request::Request(const Request &val)
{
	*this = val;
}

Request & Request::operator=(const Request &val)
{
	if (this != &val)
	{
		this->_method = val._method;
		this->_path = val._path;
		this->_current_size = val._current_size;
		this->_needed_size = val._needed_size;
		this->_finished = val._finished;
		this->_header_finished = val._header_finished;
		this->_buffer = val._buffer;
		this->_fd = val._fd;
		this->_content_length = val._content_length;
		this->_content_type = val._content_type;
	}
	return *this;
}

Request::~Request()
{
}

void	Request::receive()
{
	if (this->_fd < 0)
		throw std::runtime_error("trying to read request with no fd");
	char _buf[DEFAULT_READ];
	ssize_t bytes = recv(this->_fd, _buf, (DEFAULT_READ - 1), 0);
	if (bytes < 0)
	{
		perror("recv");
		throw std::runtime_error("failed to read request: recv error");
	}
	_buf[bytes] = '\0';
	_buffer += _buf;
}

void	Request::check_buffer()
{
	size_t header_end_pos = 0;
	if ((header_end_pos = _buffer.find(separator)) != std::string::npos)
	{
		_header = _buffer.substr(0, header_end_pos);
		_body = _buffer.substr(header_end_pos + separator.length());

		std::cout << "\nheader_len: " << _header.length() << "\n";
		std::cout << "-------------------\n "<< _header << "\n--------------------\n";
		std::cout << "\nbody_len: " << _body.length() << "\n";
		std::cout << "-------------------\n "<< _body << "\n--------------------\n";
	}
	if (header_end_pos != std::string::npos)
		_header_finished = true;
}

std::string	Request::find_field(const std::string &field_name)
{
	size_t	field_pos = _buffer.find(field_name);
	if (field_pos != std::string::npos)
	{
		std::string	field;

		field_pos += field_name.length();
		size_t field_end = _buffer.find("\r\n", field_pos);
		field = _buffer.substr(field_pos, field_end - field_pos);
		std::cout << field_name << ": " << "[" << field << "]" << std::endl;
		return field;
	}
	return "n/a";
}

void	Request::handle_header()
{
	std::string	first_line;
	size_t pos = 0;
	if ((pos = _buffer.find('\n')) == std::string::npos)
		throw std::runtime_error("no line found in request");

	first_line = _buffer.substr(0, pos);
	std::vector<std::string> tokens = ws_split(first_line, ' ');
	if (tokens.size() < 3)
		throw std::runtime_error("request received has invalid request line");
	for (int i = 0; i <= LAST; i++)
	{
		if (tokens[0] == Request::_methods_arr[i])
			this->_method = static_cast<method>(i);
	}
	this->_path = tokens[1];


	std::cout << "REQUEST METHOD: " << this->_method << ", PATH: " << this->_path << std::endl;

	// std::cout << "------REST-------\n" << req << "\n";

	_content_type = find_field(Defaults::Fields()[CONTENT_TYPE]);

	_content_length = std::atoll(find_field(Defaults::Fields()[CONTENT_TYPE]).c_str());

	// if no content_length and if read the totality of header,
	// then we know we have finished reading.
	if (_content_length == 0)
		this->_finished = true;
}

void Request::handle_body()
{
	if (_body.length() == _content_length)
		_finished = true;
}

void	Request::read()
{
	receive();
	check_buffer();
	if (_header_finished == true)
	{
		handle_header();
		if (_finished == true)
			return ;
		handle_body();
		//handle check for body
	}
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
	return Request::_methods_arr[_method];
}

bool Request::get_finished() const
{
	return _finished;
}

SOCKET	Request::get_sockfd() const
{
	return _fd;
}