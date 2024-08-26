/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:00:35 by okoca             #+#    #+#             */
/*   Updated: 2024/08/26 11:05:29by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "defaults.hpp"
#include <cstddef>
#include <cstring>
#include <string>

namespace http
{
	const char *CRLF = "\r\n";
	const char *LF = "\n";

	const char *CRLF_END = "\r\n\r\n";
	const char *LF_END = "\n\n";

	Parser::Parser(const std::string &buffer)
	{
		_raw = buffer;
	}

	bool Parser::check_exists(HeaderField::Value val) const
	{
		const char	*header_val = Defaults::get_header_field(val);
		if (_header.find(header_val) == std::string::npos)
			return false;
		return true;
	}

	void Parser::add_start_line(const std::string &line)
	{
		if (check_exists(HeaderField::VERSION))
			return ;
		_header.insert(0, line);
	}

	size_t	Parser::find_header_end(const std::string &s) const
	{
		size_t	pos = s.find(CRLF_END);
		if (pos != std::string::npos)
			return pos + std::strlen(CRLF_END);

		pos = s.find(LF_END);
		if (pos != std::string::npos)
			return pos + std::strlen(LF_END);

		return std::string::npos;
	}

	size_t	Parser::find_line_end(const std::string &s, size_t start_pos) const
	{
		size_t	pos = s.find(CRLF, start_pos);
		if (pos != std::string::npos)
			return pos;

		pos = s.find(LF, start_pos);
		if (pos != std::string::npos)
			return pos;

		return std::string::npos;
	}

	std::string Parser::get_value(HeaderField::Value val) const
	{
		const char	*header_val = Defaults::get_header_field(val);
		size_t	pos = _header.find(header_val);
		if (pos == std::string::npos)
			return "";

		size_t	new_line_pos = find_line_end(_header, pos);
		size_t	value_pos = pos + std::strlen(header_val);
		size_t	total = new_line_pos - value_pos;
		return _header.substr(value_pos, total);
	}

	std::string Parser::get_combine()
	{
		std::string	combine;
		combine.reserve(_header.size() + _body.size() + std::strlen(CRLF));

		combine.append(_header);
		combine.append(CRLF);
		combine.append(_body);
		return combine;
	}

	Parser::~Parser()
	{
	}

}
