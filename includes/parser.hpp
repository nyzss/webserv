/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 08:40:20 by okoca             #+#    #+#             */
/*   Updated: 2024/08/26 21:34:16 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
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
				LF
			};
		};
	private:
		static const char *CRLF_END;
		static const char *LF_END;
		static const char *CRLF;
		static const char *LF;

	private:
		std::string			_header;
		std::string			_body;
		std::string			_raw;

		Separator::Value	_sep;

	public:
		Parser ();
		Parser (const std::string &buffer);
		Parser (const Parser &val);
		Parser &operator=(const Parser &val);
		~Parser ();

	public:
		void add_start_line(StatusCode::Value code);
		void add_header_line(HeaderField::Value field, const std::string &line);

		void set_header_line(HeaderField::Value field, const std::string &line);
		void add_body(const std::string &body);

	public:
		std::string get_value(HeaderField::Value val) const;
		std::string get_combine() const;
		size_t		length() const;

	private:
		size_t		find_header_end(const std::string &s);

		size_t		find_line_end(const std::string &s, size_t start_pos) const;
		bool		check_exists(HeaderField::Value val) const;
	};
}

#endif /* PARSER_HPP */
