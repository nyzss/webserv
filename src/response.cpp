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

namespace http
{
	Response::Response(Request &req): _req(req)
	{
		_resource_exists = false;
		_prefix = "example";
		_cgi = false;
		_fd = -1;
	}

	void Response::send(bool cgi, const std::string &cgi_buffer, StatusCode::Value cgi_status)
	{
		this->_fd = _req.get_sockfd();
		if (_fd < 0)
			throw std::runtime_error("response object initialised with invalid fd");

		if (cgi_status != StatusCode::OK)
		{
			_message = Parser(cgi_status);
		}
		else if (cgi)
		{
			_message = cgi_buffer;
			_message.add_start_line(StatusCode::OK);
			_message.append(HeaderField::CONNECTION, "close");
		}
		else
		{
			init_resource();
			builder();
		}
		write();
	}

	void Response::builder()
	{
		std::string	ext = get_extension(_req.get_path());
		if (!_resource_exists || ext == _req.get_path() || ext == "html")
			_message.append(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::HTML));
		else if (ext == "css")
			_message.append(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::CSS));
		else if (ext == "js")
			_message.append(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::JAVASCRIPT));
		else if (ext == "jpg")
			_message.append(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::IMAGE_JPEG));
		else if (ext == "png")
			_message.append(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::IMAGE_PNG));
		else if (ext == "gif")
			_message.append(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::IMAGE_GIF));
		else
			_message.append(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::HTML));

		_message.append(HeaderField::CONNECTION, "close");

		if (_resource_exists)
			_message.add_start_line(StatusCode::OK);
		else
			_message.add_start_line(StatusCode::NOT_FOUND);
	}

	void Response::write()
	{
		debug();
		std::string	combine = _message.generate();
		ssize_t r_sd = ::send(_fd, combine.data(), combine.length(), 0);
		if (r_sd < 0)
			throw std::runtime_error("SHOULD SEND BACK SERVER ERROR HERE 500");
	}

	void Response::read_file(std::ifstream &file)
	{
		if (!_resource_exists || !file)
		{
			_message.add_body(Defaults::html_not_found);
			return ;
		}
		std::stringstream stream;
		stream << file.rdbuf();
		_message.add_body(stream.str());
	}

	void Response::init_resource()
	{
		std::string filename = _prefix;
		std::ifstream	file;

		filename += _req.get_path();

		file.open(filename.c_str(), std::ios::binary);
		if (file.good())
			_resource_exists = is_file(filename);
		read_file(file);
	}

	void Response::debug() const
	{
		std::cout << "\n\n--------------- RESPONSE ---------------\n";
		_message.display_header();
	}

	Response & Response::operator=(const Response &val)
	{
		if (this != &val)
		{
			this->_fd = val._fd;
			this->_req = val._req;
			this->_buffer = val._buffer;

			this->_prefix = val._prefix;
			this->_resource_exists = val._resource_exists;

			this->_message = val._message;
		}
		return *this;
	}

	Response::~Response()
	{}

	Response::Response(const Response &val) : _req(val._req)
	{
		*this = val;
	}
}
