/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:47:10 by okoca             #+#    #+#             */
/*   Updated: 2024/09/08 10:25:57 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

namespace http
{
	Client::Client() : Socket(), _res(_req), _server_sock(-1), _sock_len(sizeof(_data))
	{
		std::memset(&this->_data, 0, sizeof(this->_data));
	}

	Client::Client(SOCKET server_sock) : Socket(), _res(_req), _server_sock(server_sock), _sock_len(sizeof(_data))
	{
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

		// std::cout << "-------CLIENT DATA--------" << std::endl;
		// std::cout << "socklen_t: " << this->_sock_len << std::endl;
		// std::cout << "sin_addr: " << inet_ntoa(this->_data.sin_addr) << std::endl;
		// std::cout << "port: " << ntohs(this->_data.sin_port) << std::endl;
		// std::cout << "family: " << this->_data.sin_family << std::endl;
		// std::cout << "-----------END---------" << std::endl;
	}

	Client::Client(const Client &value) : Socket(value), _res(_req)
	{
		this->_server_sock = value._server_sock;
		this->_sock_len = value._sock_len;
	}

	Client::~Client()
	{}

	bool	Client::request()
	{
		return _req.read();
	}

	void	Client::response()
	{
		_res.send();
	}

	void	Client::reset()
	{
		if (_fd != -1)
			close(_fd);
		_fd = -1;
	}

	bool	Client::get_finished() const
	{
		return _req._finished;
	}
}