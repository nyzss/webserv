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
#include "utils.hpp"
#include <cstddef>
#include <cstring>
#include <string>

namespace http
{
	const char *Parser::CRLF = "\r\n";
	const char *Parser::LF = "\n";

	const char *Parser::CRLF_END = "\r\n\r\n";
	const char *Parser::LF_END = "\n\n";

	Parser::Parser()
	{
		_sep = Separator::CRLF;
	}

	Parser::Parser(const std::string &buffer)
	{
		_raw = buffer;
		size_t	header_end = find_header_end(_raw);

		if (header_end == std::string::npos)
			_sep = Separator::CRLF;
		else
		{
			_header = buffer.substr(0, header_end);
			if (_sep == Separator::CRLF)
				header_end += std::strlen(CRLF);
			else
				header_end += std::strlen(LF);
			_body = buffer.substr(header_end);
		}
	}

	bool Parser::check_exists(HeaderField::Value val) const
	{
		const char	*header_val = Defaults::get_header_field(val);
		if (_header.find(header_val) == std::string::npos)
			return false;
		return true;
	}

	void Parser::add_start_line(StatusCode::Value code)
	{
		if (check_exists(HeaderField::VERSION))
			return ;
		const char *h = Defaults::get_header_field(HeaderField::VERSION);
		std::string status_code = Defaults::get_status_code(code);
		_header.insert(0, h + status_code + CRLF);
	}

	void Parser::add_header_line(HeaderField::Value field, const std::string &line)
	{
		if (check_exists(field))
			return ;
		const char * f = Defaults::get_header_field(field);
		_header.append(f + line + CRLF);
	}

	void Parser::set_header_line(HeaderField::Value field, const std::string &line)
	{
		const char *s = Defaults::get_header_field(field);
		size_t	pos = _header.find(s);
		if (pos == std::string::npos)
			add_header_line(field, line);
		else
		{
			std::string val_to_replace = get_value(field);
			size_t val_pos = _header.find(val_to_replace);

			_header.replace(val_pos, val_to_replace.length(), line + CRLF);
		}
	}

	void Parser::add_body(const std::string &body)
	{
		_body = body;
		set_header_line(HeaderField::CONTENT_LENGTH, to_string(_body.length()));
	}

	size_t	Parser::find_header_end(const std::string &s)
	{
		size_t	pos = s.find(CRLF_END);
		if (pos != std::string::npos)
		{
			_sep = Separator::CRLF;
			return pos;
		}

		pos = s.find(LF_END);
		if (pos != std::string::npos)
		{
			_sep = Separator::LF;
			return pos;
		}
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
	{ }

}
