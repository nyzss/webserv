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
#include "webserv.hpp"

namespace http
{
	const char *Parser::CRLF = "\r\n";
	const char *Parser::LF = "\n";

	const char *Parser::CRLF_END = "\r\n\r\n";
	const char *Parser::LF_END = "\n\n";

	Parser::Parser()
	{
		_sep = Separator::NONE;
	}

	Parser::Parser (const Parser &val)
	{
		*this = val;
	}

	Parser::Parser(const std::string &buffer)
	{
		_sep = Separator::NONE;
		handle_buffer(buffer);
	}

	Parser &Parser::operator=(const Parser &val)
	{
		if (this != &val)
		{
			_body = val._body;
			_sep = val._sep;
			_header_fields = val._header_fields;
			_start_line = val._start_line;
		}
		return *this;
	}

	void Parser::append(HeaderField::Value val, const std::string &value)
	{
		append(Defaults::get_header_field(val), value);
	}

	void Parser::append(const std::string &key, const std::string &value)
	{
		_header_fields[key].push_back(value);
	}

	void Parser::set_start_line(const std::string &s)
	{
		_start_line = s;
	}

	void Parser::parse_buffer(std::string buf)
	{
		size_t	nl_cpy = buf.find(LF);

		if (buf.find(Defaults::get_header_field(HeaderField::VERSION)) != std::string::npos)
		{
			_start_line = buf.substr(0, nl_cpy);
			buf.erase(0, nl_cpy + 1);
		}

		while ((nl_cpy = buf.find(LF)) != std::string::npos)
		{
			// simple field may look like:  [Content-Type: text/html\n]
			std::string	header_key_val = buf.substr(0, nl_cpy);
			if (header_key_val.size() == 0)
				break ;

			size_t	sep_pos = header_key_val.find(":");
			std::string	key = header_key_val.substr(0, sep_pos);
			std::string	val = header_key_val.substr(sep_pos + 2);

			_header_fields[key].push_back(val);

			buf.erase(0, nl_cpy + 1); // removes the LF too
		}
	}

	void Parser::handle_buffer(const std::string &buf)
	{
		size_t		header_end = find_header_end(buf);

		if (header_end == std::string::npos)
			return ;

		size_t	sep_offset = _sep == Separator::CRLF ? std::strlen(CRLF_END) : std::strlen(LF_END);
		std::string	parsed_header = buf.substr(0, header_end + sep_offset);
		add_body(buf.substr(header_end + sep_offset));

		size_t	nl_pos = 0;
		while ((nl_pos = parsed_header.find(CRLF)) != std::string::npos)
		{
			parsed_header.replace(nl_pos, 2, LF);
		}

		parse_buffer(parsed_header);

		// // for (size_t i = 0; i < parsed_header.length(); i++)
		// // {
		// // 	if (parsed_header[i] == '\r')
		// // 		std::cout << "\\r";
		// // 	else if (parsed_header[i] == '\n')
		// // 		std::cout << "\\n" << "\n";
		// // 	else
		// // 		std::cout << parsed_header[i];
		// // }
		// // std::cout << std::endl;
		// std::cout << "parsed http: " << parsed_header << std::endl;

		// std::cout << "GENERATED HTTP: " << generate() << std::endl;
	}

	void Parser::add_start_line(StatusCode::Value code)
	{
		_start_line = Defaults::get_header_field(HeaderField::VERSION);
		_start_line.append(" ").append(Defaults::get_status_code(code));
	}

	void Parser::add_body(const std::string &body)
	{
		_body = body;
		append(Defaults::get_header_field(HeaderField::CONTENT_LENGTH), to_string(_body.length()));
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

	size_t	Parser::length() const
	{
		return generate().length();
	}

	const std::string &Parser::get_body() const
	{
		return _body;
	}

	std::string Parser::generate() const
	{
		std::string	combination;

		combination.append(generate_header());
		combination.append(CRLF);
		combination.append(_body);
		return combination;
	}

	void Parser::display_header() const
	{
		std::cout << "START_LINE: [" << _start_line << "]\n";
		std::map<std::string, std::vector<std::string> >::const_iterator it;
		for (it = _header_fields.begin(); it != _header_fields.end(); it++)
		{
			std::cout << "- KEY: {" << (*it).first << "}\n";
			std::vector<std::string>::const_iterator f;
			for (f = (*it).second.begin(); f != (*it).second.end(); f++)
			{
				std::cout << "\t[" << (*f) << "]\n";
			}
		}
		std::cout << std::endl;
	}

	std::string Parser::generate_header() const
	{
		std::string	header;
		header.append(_start_line + CRLF);

		std::map<std::string, std::vector<std::string> >::const_iterator map_it;
		std::vector<std::string>::const_iterator vec_it;
		for (map_it = _header_fields.begin(); map_it != _header_fields.end(); map_it++)
		{
			for (vec_it = (*map_it).second.begin(); vec_it != (*map_it).second.end(); vec_it++)
			{
				std::string header_line = (*map_it).first + ": " + *vec_it + CRLF;
				header.append(header_line);
			}
		}
		return header;
	}

	bool Parser::exist(const std::string &s) const
	{
		if (_header_fields.find(s) != _header_fields.end())
			return true;
		return false;
	}

	std::string Parser::get_header_value(const std::string &s) const
	{
		if (_header_fields.find(s) == _header_fields.end())
			return "";

		return _header_fields.at(s).front();
	}

	const std::string &Parser::get_start_line() const
	{
		return _start_line;
	}

	bool Parser::get_finished() const
	{
		return _sep != Separator::NONE;
	}

	Parser::~Parser()
	{ }

}
