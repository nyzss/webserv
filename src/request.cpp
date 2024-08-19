/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:17:05 by okoca             #+#    #+#             */
/*   Updated: 2024/08/19 09:30:22 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request() : _method(NONE)
{
}

Request::Request(const std::string &req)
{
}

Request::Request(const Request &val)
{
	this->_method = val._method;
	this->_path = val._path;
}

Request::~Request()
{
}

Request & Request::operator=(const Request &val)
{
	if (this != &val)
	{
		this->_method = val._method;
		this->_path = val._path;
	}
	return *this;
}
