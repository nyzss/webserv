/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 08:40:20 by okoca             #+#    #+#             */
/*   Updated: 2024/08/27 14:58:26 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#ifndef PARSER_HPP
# define PARSER_HPP

#include <common.hpp>
#include <defaults.hpp>

namespace http
{
	class Parser
	{
	private:
		struct Separator
		{
			enum Value
			{
				CRLF,
				LF,
				NONE
			};
		};
	private:
		static const char *CRLF_END;
		static const char *LF_END;
		static const char *CRLF;
		static const char *LF;

	private:
		std::string			_body;

		std::string			_start_line;
		Separator::Value	_sep;
		std::map<std::string, std::vector<std::string> > _header_fields;

	public:
		Parser ();
		Parser (const std::string &buffer);
		Parser (const Parser &val);
		Parser &operator=(const Parser &val);
		~Parser ();

	public:
		void add_start_line(StatusCode::Value code);
		void add_body(const std::string &body);

	public:
		void append(const std::string &key, const std::string &value);
		void append(HeaderField::Value val, const std::string &value);

		size_t		length() const;
		std::string generate() const;
		std::string generate_header() const;

		void display_header() const;

		const std::string &get_body() const;
		const std::string &get_start_line() const;
		std::string get_header_value(const std::string &s) const;
		bool exist(const std::string &s) const;

		bool get_finished() const;

	private:
		size_t		find_header_end(const std::string &s);

		void handle_buffer(const std::string &buf);
		void parse_buffer(std::string buf);
		void set_start_line(const std::string &s);
	};
}

#endif /* PARSER_HPP */
