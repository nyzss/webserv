/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaults.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:25:49 by okoca             #+#    #+#             */
/*   Updated: 2024/08/26 08:59:55 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defaults.hpp"
#include <stdexcept>
#include <webserv.hpp>

namespace http
{
	const std::string	Defaults::html_not_found =	"<html><body>"
													"<h1>404 - Not Found</h1>"
													"</html></body>";

	const char *Defaults::_status[] =
	{
		"200 OK",
		"404 Not Found",
	};

	const char *Defaults::_fields[] =
	{
		"HTTP/1.1 ",
		"Content-Type: ",
		"Content-Length: ",
		"Connection: ",
	};

	const char *Defaults::_content_type[] =
	{
		"text/html",
		"text/css",
		"application/javascript",
		"multipart/form-data",
		"application/octet-stream",
		"image/jpeg",
		"image/png",
		"image/webp",
		"image/gif",
	};

	const char	*Defaults::get_status_code(StatusCode::Value code)
	{
		if (code > StatusCode::COUNT)
			throw std::out_of_range("fail: no status code matching");
		return _status[code];
	}

	const char	*Defaults::get_header_field(HeaderField::Value field)
	{
		if (field > HeaderField::COUNT)
			throw std::out_of_range("fail: no header field matching");
		return _fields[field];
	}

	const char	*Defaults::get_content_type(ContentType::Value type)
	{
		if (type > ContentType::COUNT)
			throw std::out_of_range("fail: no content type matching");
		return _content_type[type];
	}
}