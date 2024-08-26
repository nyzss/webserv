/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 08:40:20 by okoca             #+#    #+#             */
/*   Updated: 2024/08/26 13:21:48 by okoca            ###   ########.fr       */
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
		static const char *CRLF_END;
		static const char *LF_END;
		static const char *CRLF;
		static const char *LF;

	private:
		std::string	_header;
		std::string	_body;
		std::string	_raw;

	public:
		Parser (const std::string &buffer);
		~Parser ();

	public:
		void add_start_line(const std::string &line);

	public:
		std::string get_value(HeaderField::Value val) const;
		std::string get_combine();

	private:
		size_t		find_header_end(const std::string &s) const;
		size_t		find_line_end(const std::string &s, size_t start_pos) const;
		bool		check_exists(HeaderField::Value val) const;
	};
}

#endif /* PARSER_HPP */
