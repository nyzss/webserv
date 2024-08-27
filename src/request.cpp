/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:17:05 by okoca             #+#    #+#             */
/*   Updated: 2024/08/27 15:31:39 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defaults.hpp"
#include <cstddef>
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
			this->_message = val._message;
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
		_message = _buffer;
		_header_finished = _message.get_finished();
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

	void Request::handle_body()
	{
		const std::string &content_len = _message.get_header_value(Defaults::get_header_field(HeaderField::CONTENT_LENGTH));
		size_t	len = std::atoll(content_len.c_str());
		if (_message.get_body().length() == len || len == 0)
			_finished = true;
	}

	void	Request::read()
	{
		receive();
		check_buffer();
		if (_header_finished)
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
		handle_raw_bytes_post(filename, _message.get_body());
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
		std:: cout << "\n-----------------REQUEST---------------\n" << std::endl;

		_message.display_header();
		std::cout << "BODY_LEN: " << _message.get_body().length() << std::endl;;
	}
}