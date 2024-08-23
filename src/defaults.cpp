/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaults.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:25:49 by okoca             #+#    #+#             */
/*   Updated: 2024/08/23 14:46:57 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

const std::string	Defaults::html_not_found =	"<html><body>"
												"<h1>404 - Not Found</h1>"
												"</html></body>";

const char *Defaults::_status[STATUS_COUNT] =
{
	"200 OK",
	"404 Not Found",
};

const char *Defaults::_fields[FIELD_COUNT] =
{
	"HTTP/1.1 ",
	"Content-Type: ",
	"Content-Length: ",
	"Connection: ",
};

Defaults::~Defaults() {}