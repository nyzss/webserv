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
#include <exception>
#include <string>
#include <utility>

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

	Parser::Parser (const Parser &val)
	{
		*this = val;
	}

	Parser &Parser::operator=(const Parser &val)
	{
		if (this != &val)
		{
			this->_raw = val._raw;
			this->_header = val._header;
			this->_body = val._body;
			this->_sep = val._sep;
		}
		return *this;
	}

	void Parser::normalize(const std::string &buf)
	{
		size_t		header_end = find_header_end(buf);

		if (header_end == std::string::npos)
			return ;

		size_t	sep_offset = _sep == Separator::CRLF ? std::strlen(CRLF_END) : std::strlen(LF_END);
		std::string	parsed_header = buf.substr(0, header_end + sep_offset);

		size_t	nl_pos = 0;
		while ((nl_pos = parsed_header.find(CRLF)) != std::string::npos)
		{
			parsed_header.replace(nl_pos, 2, LF);
		}

		std::string	start_line;

		std::string header_cpy = parsed_header;
		size_t	nl_cpy = header_cpy.find(LF);


		start_line = header_cpy.substr(0, nl_cpy);
		header_cpy.erase(0, nl_cpy + 1);

		while ((nl_cpy = header_cpy.find(LF)) != std::string::npos)
		{
			// simple field may look like:  [Content-Type: text/html\n]
			std::string	header_key_val = header_cpy.substr(0, nl_cpy);
			if (header_key_val.size() == 0)
				break ;

			size_t	sep_pos = header_key_val.find(":");
			std::string	key = header_key_val.substr(0, sep_pos);
			std::string	val = header_key_val.substr(sep_pos + 2);

			_header_fields[key].push_back(val);

			header_cpy.erase(0, nl_cpy + 1); // removes the LF too
		}

		std::cout << "START_LINE: [" << start_line << "]\n";
		std::map<std::string, std::vector<std::string> >::const_iterator it;
		for (it = _header_fields.begin(); it != _header_fields.end(); it++)
		{
			std::cout << "key: [" << (*it).first << "]\n";
			std::cout << "pairs: ";
			std::vector<std::string>::const_iterator f;
			for (f = (*it).second.begin(); f != (*it).second.end(); f++)
			{
				std::cout << "\t[" << (*f) << "]\n";
			}
		}
		std::cout << std::endl;

		for (size_t i = 0; i < parsed_header.length(); i++)
		{
			if (parsed_header[i] == '\r')
				std::cout << "\\r";
			else if (parsed_header[i] == '\n')
				std::cout << "\\n" << "\n";
			else
				std::cout << parsed_header[i];
		}
		std::cout << std::endl;
	}

	Parser::Parser(const std::string &buffer)
	{
		normalize(buffer);
		_raw = buffer;
		size_t	header_end = find_header_end(_raw);

		if (header_end == std::string::npos)
			_sep = Separator::CRLF;
		else
		{
			_header = _raw.substr(0, header_end);
			_header.append(CRLF);

			size_t	sep_offset = _sep == Separator::CRLF ? std::strlen(CRLF_END) : std::strlen(LF_END);
			add_body(_raw.substr(header_end + sep_offset));
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
		if (!check_exists(field))
			add_header_line(field, line);

		std::string val_to_replace = get_value(field);
		size_t val_pos = _header.find(val_to_replace);

		_header.replace(val_pos, val_to_replace.length(), line);
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

	std::string Parser::get_combine() const
	{
		std::string	combine;
		combine.reserve(_header.size() + _body.size() + std::strlen(CRLF));

		combine.append(_header);
		combine.append(CRLF);
		combine.append(_body);
		return combine;
	}
	size_t	Parser::length() const
	{
		return _header.size() + _body.size() + std::strlen(CRLF);
	}

	const std::string &Parser::get_header() const
	{
		return _header;
	}

	const std::string &Parser::get_body() const
	{
		return _body;
	}

	Parser::~Parser()
	{ }

}
