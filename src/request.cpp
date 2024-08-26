/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:17:05 by okoca             #+#    #+#             */
/*   Updated: 2024/08/26 09:12:24 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defaults.hpp"
#include <webserv.hpp>

namespace http
{
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
			this->_header = val._header;
			this->_body = val._body;
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
		ssize_t bytes = recv(this->_fd, _buf, DEFAULT_READ, 0);
		if (bytes < 0)
		{
			perror("recv");
			throw std::runtime_error("failed to read request: recv error");
		}
		_buffer.append(_buf, bytes);
	}

	void	Request::check_buffer()
	{
		size_t header_end_pos = _buffer.find(separator);
		if (!_header_finished)
		{
			if (header_end_pos == std::string::npos)
				return;
			_header_finished = true;
		}
		_header = _buffer.substr(0, header_end_pos);
		_body = _buffer.substr(header_end_pos + separator.length());
	}

	std::string	Request::find_field(const std::string &field_name)
	{
		size_t	field_pos = _header.find(field_name);
		if (field_pos != std::string::npos)
		{
			std::string	field;

			field_pos += field_name.length();
			size_t field_end = _header.find("\r\n", field_pos);
			field = _header.substr(field_pos, field_end - field_pos);
			// std::cout << field_name << ": " << "[" << field << "]" << std::endl;
			return field;
		}
		return "n/a";
	}

	void	Request::handle_header()
	{
		std::string	first_line;
		size_t pos = 0;
		if ((pos = _header.find('\n')) == std::string::npos)
			throw std::runtime_error("no line found in request");

		first_line = _header.substr(0, pos);
		std::vector<std::string> tokens = ws_split(first_line, ' ');
		if (tokens.size() < 3)
			throw std::runtime_error("request received has invalid request line");
		for (int i = 0; i <= LAST; i++)
		{
			if (tokens[0] == Request::_methods_arr[i])
				this->_method = static_cast<method>(i);
		}
		if (tokens[1] == "/")
			this->_path = "/index.html";
		else
			this->_path = tokens[1];

		_content_type = find_field(Defaults::get_header_field(http::HeaderField::CONTENT_TYPE));
		_content_length = std::atoll(find_field(Defaults::get_header_field(http::HeaderField::CONTENT_LENGTH)).c_str());
	}

	void Request::handle_body()
	{
		if (_body.length() == _content_length || _content_length == 0)
			_finished = true;
	}

	void	Request::read()
	{
		receive();
		check_buffer();
		if (_header_finished == true)
		{
			handle_header();
			handle_body();
		}
		if (_finished)
		{
			debug();
			handle_post();
		}
	}

	// for simple post requests, to write the whole body inside output file
	void Request::handle_raw_bytes_post(const char *filename) const
	{
		handle_raw_bytes_post(filename, _body);
	}

	// for more complex post requests, only writes the `data` inside the output file
	void Request::handle_raw_bytes_post(const char *filename, const std::string &data) const
	{
		std::string output(UPLOADED);
		output += filename;
		std::ofstream output_file(output.c_str());

		if (!output_file)
			throw std::runtime_error("couldnt open output file!");

		if (!output_file.write(data.data(), data.size()))
			throw std::runtime_error("couldnt write data to output file!");
	}

	void Request::handle_post() const
	{
		if (_method != POST)
			return ;

		if (_content_type.find(Defaults::get_content_type(ContentType::FORMDATA)) != std::string::npos)
			std::cout << "MULTIPART_FOMR_DATA POST REACHED()" << std::endl;
		else if (_content_type == Defaults::get_content_type(ContentType::OCTEC_STREAM))
		{
			handle_raw_bytes_post("raw_data");
			std::cout << "OCTET_STREAM REACHED" << std::endl;
		}
		else if (_content_type == Defaults::get_content_type(ContentType::IMAGE_JPEG))
		{
			handle_raw_bytes_post("img.jpg");
			std::cout << "IMAGE_JPEG REACHED" << std::endl;
		}
		else if (_content_type == Defaults::get_content_type(ContentType::IMAGE_PNG))
		{
			handle_raw_bytes_post("img.png");
			std::cout << "IMAGE_PNG REACHED" << std::endl;
		}
		else if (_content_type == Defaults::get_content_type(ContentType::IMAGE_WEBP))
		{
			handle_raw_bytes_post("img.webp");
			std::cout << "IMAGE_WEBP REACHED" << std::endl;
		}
		else if (_content_type == Defaults::get_content_type(ContentType::IMAGE_GIF))
		{
			handle_raw_bytes_post("img.gif");
			std::cout << "IMAGE_GIF REACHED" << std::endl;
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

	void Request::debug() const
	{
		std::cout << "\nheader_len: " << _header.length() << "\n";
		std::cout << "-------------------\n "<< _header << "\n--------------------\n";
		std::cout << "\nbody_len: " << _body.length() << "\n";
		// std::cout << "-------------------\n "<< _body << "\n--------------------\n";


		std::cout << "content_len: " << _content_length << "\nbody_len: " << _body.size() << "\n";
		// std::ofstream s("output.data");
		// s.write(_body.data(), _body.size());
		std:: cout << "-----------------\nFINISHED\n---------------\n" << std::endl;
	}
}