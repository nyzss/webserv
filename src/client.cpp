/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:47:10 by okoca             #+#    #+#             */
/*   Updated: 2024/09/10 14:56:20 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

namespace http
{
	Client::Client() : Socket(), _res(_req)
	{
		_server_sock = -1;
		_sock_len = sizeof(_data);
		_cgi = false;
		_pipe = -1;
		_cgi_status = StatusCode::OK;
		std::memset(&this->_data, 0, sizeof(this->_data));
	}

	Client::Client(SOCKET server_sock) : Socket(), _res(_req)
	{
		_server_sock = server_sock;
		_sock_len = sizeof(_data);
		_cgi = false;
		_pipe = -1;
		_cgi_status = StatusCode::OK;
		get_connection();
	}

	void	Client::get_connection()
	{
		if (_server_sock == -1)
			throw std::runtime_error("no server socket on client");

		this->_fd = accept(_server_sock,
			(sockaddr *)&this->_data, &this->_sock_len);
		if (this->_fd < 0)
			throw std::runtime_error("accept error on client");
		non_blocking();
		this->_req = Request(_fd);
	}

	Client::Client(const Client &value) : Socket(value), _res(_req)
	{
		this->_server_sock = value._server_sock;
		this->_sock_len = value._sock_len;

		this->_cgi = value._cgi;
		this->_cgi_buffer = value._cgi_buffer;
		this->_pipe = value._pipe;
		this->_cgi_status = value._cgi_status;
	}

	Client::~Client()
	{
		if (_pipe != -1)
			close(_pipe);
	}

	void Client::cgi_handler(const std::string &cgi)
	{
		char	*args[3];
		int		fd[2];

		std::string	_prefix = "example";

		try
		{
			std::string	exec = "/usr/bin/" + cgi;
			if (!is_file(exec) || access(exec.c_str(), X_OK | F_OK) != 0)
				throw Exceptions::ExecutableNotFound();

			std::string file = _prefix + _req.get_path();
			if (!is_file(file))
				throw Exceptions::CGINotFound();

			if (pipe(fd) != 0)
				throw std::runtime_error("failed to initialise pipes for cgi");
			pid_t pid = fork();
			if (pid < 0)
				throw std::runtime_error("failed to fork process for cgi");
			if (pid == 0)
			{
				args[0] = const_cast<char *>(exec.c_str());
				args[1] = const_cast<char*>(file.c_str());
				args[2] = NULL;
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
				close(fd[0]);
				execve(args[0], args, environ);
				throw Exceptions::Unhandled();
			}
			close(fd[1]);
			_pipe = fd[0];
			set_non_blocking(_pipe);
		}
		catch (Exceptions::ExecutableNotFound)
		{
			_cgi_status = StatusCode::INTERNAL_SERVER_ERROR;
		}
		catch (Exceptions::CGINotFound)
		{
			_cgi_status = StatusCode::NOT_FOUND;
		}
		catch (std::exception &e)
		{
			throw Exceptions::Unhandled();
		}
	}

	bool	Client::request()
	{
		bool	finished = _req.read();

		if (finished)
		{
			std::string	ext = get_extension(_req.get_path());
			if (ext == "py")
			{
				cgi_handler("python");
				if (_cgi_status == StatusCode::OK)
					_cgi = true;
			}
		}

		return finished;
	}

	bool	Client::cgi()
	{
		char buf[DEFAULT_READ];
		size_t b_read = read(_pipe, buf, DEFAULT_READ);
		if (b_read == 0)
			return true;
		else if (b_read > 0)
		{
			_cgi_buffer.append(buf, b_read);
			Parser p(_cgi_buffer);
			if (p.get_finished())
			{
				if (!p.match_content_len())
				{
					_cgi_status = StatusCode::INTERNAL_SERVER_ERROR;
				}
				return true;
			}
		}
		else if (b_read < 0)
			throw std::runtime_error("err: cgi pipe read error");
		return false;
	}

	void	Client::response()
	{
		_res.send(_cgi, _cgi_buffer, _cgi_status);
	}

	void	Client::reset()
	{
		brute_close(_pipe);
	}

	PIPE	Client::get_pipe_fd() const
	{
		return _pipe;
	}

	bool	Client::has_cgi() const
	{
		return _cgi;
	}

	void	Client::debug() const
	{
		std::cout << "-------CLIENT DATA - [ " << this->_fd <<  " ]--------" << std::endl;
		std::cout << "socklen_t: " << this->_sock_len << std::endl;
		std::cout << "sin_addr: " << inet_ntoa(this->_data.sin_addr) << std::endl;
		std::cout << "port: " << ntohs(this->_data.sin_port) << std::endl;
		std::cout << "family: " << this->_data.sin_family << std::endl;
		std::cout << "-----------END---------" << std::endl;
	}
}