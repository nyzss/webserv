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
		const char *status_code = Defaults::get_status_code(code);
		add_header_line(HeaderField::VERSION, status_code, 0);
	}

	void Parser::add_start_line(const std::string &line)
	{
		add_header_line(HeaderField::VERSION, line, 0);
	}

	void Parser::add_header_line(HeaderField::Value field, const std::string &line)
	{
		if (check_exists(field))
			return ;
		_header.append(line + CRLF);
	}

	void Parser::add_header_line(HeaderField::Value field, const std::string &line, size_t idx)
	{
		if (check_exists(field))
			return ;
		_header.insert(idx, line + CRLF);
	}

	// void Parser::set_header_line(HeaderField::Value field, const std::string &line)
	// {
	// 	size_t	pos
	// }

	void Parser::add_body(const std::string &body)
	{
		_body = body;
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
