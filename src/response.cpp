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

#include "defaults.hpp"
#include "utils.hpp"
#include <ios>
#include <sstream>
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

	void Response::send()
	{
		this->_fd = _req.get_sockfd();
		if (_fd < 0)
			throw std::runtime_error("response object initialised with invalid fd");

		check_cgi();
		if (!_cgi)
		{
			init_resource();
			builder();
		}
		write();
	}

	void Response::cgi_handler(const std::string &cgi)
	{
		char	*args[3];
		int		fd[2];

		if (pipe(fd) != 0)
			throw std::runtime_error("failed to initialise pipes for cgi");
		pid_t pid = fork();
		if (pid < 0)
			throw std::runtime_error("failed to fork process for cgi");
		if (pid == 0)
		{
			std::string	exec = "/usr/bin/" + cgi;
			std::string file = _prefix + _req.get_path();

			args[0] = const_cast<char *>(exec.c_str());
			args[1] = const_cast<char*>(file.c_str());
			args[2] = NULL;
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			close(fd[0]);
			execve(args[0], args, environ);
			throw std::runtime_error("execve failed, exit.");
		}
		close(fd[1]);

		std::string	cgi_buffer;

		char buf[DEFAULT_READ];
		int	b_read;
		while ((b_read = read(fd[0], buf, 1024)))
			cgi_buffer.append(buf, b_read);
		close(fd[0]);


		_message = cgi_buffer;
		_message.add_start_line(StatusCode::OK);
	}

	void	Response::check_cgi()
	{
		std::string	ext = get_extension(_req.get_path());
		if (ext == "py")
		{
			_cgi = true;
			cgi_handler("python3");
		}
	}

	void Response::builder()
	{
		std::string	ext = get_extension(_req.get_path());
		if (!_resource_exists || ext == _req.get_path() || ext == "html")
			_message.add_header_line(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::HTML));
		else if (ext == "css")
			_message.add_header_line(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::CSS));
		else if (ext == "js")
			_message.add_header_line(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::JAVASCRIPT));
		else if (ext == "jpg")
			_message.add_header_line(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::IMAGE_JPEG));
		else if (ext == "png")
			_message.add_header_line(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::IMAGE_PNG));
		else if (ext == "gif")
			_message.add_header_line(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::IMAGE_GIF));
		else
			_message.add_header_line(HeaderField::CONTENT_TYPE, Defaults::get_content_type(ContentType::HTML));

		_message.add_header_line(HeaderField::CONNECTION, "close");

		if (_resource_exists)
			_message.add_start_line(StatusCode::OK);
		else
			_message.add_start_line(StatusCode::NOT_FOUND);
	}

	void Response::write()
	{
		// debug();
		std::string	combine = _message.get_combine();
		int r_sd = ::send(_fd, combine.data(), combine.length(), 0);
		if (r_sd < 0)
			throw std::runtime_error("client couldn't communicate with server!");
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
		std::cout << "\n\n\n--------------- RESPONSE ---------------\n";
		std::cout << _message.get_combine().substr(0, 150);
		if (_message.length() > 150)
			std::cout << "..";
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
