/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:17:05 by okoca             #+#    #+#             */
/*   Updated: 2024/09/08 13:55:31 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "defaults.hpp"
#include <cstddef>
#include <stdexcept>
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
		_content_length = 0;
		_fd = -1;
	}

	Request::Request(SOCKET sockfd)
	{
		_fd = sockfd;
		_finished = false;
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
			this->_buffer = val._buffer;
			this->_fd = val._fd;
			this->_content_length = val._content_length;
			this->_message = val._message;
		}
		return *this;
	}

	Request::~Request()
	{
	}

	void	Request::receive()
	{
		char _buf[DEFAULT_READ];

		if (this->_fd < 0)
			throw std::runtime_error("trying to read request with no fd");

		ssize_t bytes = recv(this->_fd, _buf, DEFAULT_READ, 0);

		// if bytes == DEFAULT_READ -> then process the header once,
		// check for methods and stuff, if valid then continue
		// if not then return error and remove fd from epoll_ctl

		if (bytes > 0)
		{
			_buffer.append(_buf, bytes);
			_message = _buffer;
		}
		else if (bytes == 0)
			_finished = true;
		else if (bytes < 0)
		{
			throw std::runtime_error("recv returned < 0");
		}
	}

	void	Request::handle_header()
	{
		std::vector<std::string> tokens = ws_split(_message.get_start_line(), ' ');
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
	}

	bool	Request::read()
	{
		receive();
		if (_message.get_finished() || _finished)
		{
			if (_message.match_content_len())
			{
				_finished = true;
				handle_header();
				if (_method == POST)
					handle_post();
				debug();
				return true;
			}
		}
		return false;
	}

	// for more complex post requests, only writes the `data` inside the output file
	void Request::handle_raw_bytes_post(const char *filename) const
	{
		std::string output(UPLOADED);
		output += filename;
		std::ofstream output_file(output.c_str());

		if (!output_file)
			throw std::runtime_error("couldnt open output file!");

		if (!output_file.write(_message.get_body().data(), _message.get_body().size()))
			throw std::runtime_error("couldnt write data to output file!");
	}

	void Request::handle_post() const
	{
		const std::string &content_type = _message.get_header_value(Defaults::get_header_field(HeaderField::CONTENT_TYPE));

		if (content_type.find(Defaults::get_content_type(ContentType::FORMDATA)) != std::string::npos)
			std::cout << "MULTIPART_FOMR_DATA POST REACHED()" << std::endl;
		else if (content_type == Defaults::get_content_type(ContentType::OCTEC_STREAM))
		{
			handle_raw_bytes_post("raw_data");
			std::cout << "OCTET_STREAM REACHED" << std::endl;
		}
		else if (content_type == Defaults::get_content_type(ContentType::IMAGE_JPEG))
		{
			handle_raw_bytes_post("img.jpg");
			std::cout << "IMAGE_JPEG REACHED" << std::endl;
		}
		else if (content_type == Defaults::get_content_type(ContentType::IMAGE_PNG))
		{
			handle_raw_bytes_post("img.png");
			std::cout << "IMAGE_PNG REACHED" << std::endl;
		}
		else if (content_type == Defaults::get_content_type(ContentType::IMAGE_WEBP))
		{
			handle_raw_bytes_post("img.webp");
			std::cout << "IMAGE_WEBP REACHED" << std::endl;
		}
		else if (content_type == Defaults::get_content_type(ContentType::IMAGE_GIF))
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

	SOCKET	Request::get_sockfd() const
	{
		return _fd;
	}

	void Request::debug() const
	{
		std:: cout << "\n-----------------REQUEST---------------\n" << std::endl;

		_message.display_header();
		std::cout << "- BODY_LEN: [" << _message.get_body().length() << "]" << std::endl;;
	}
}
