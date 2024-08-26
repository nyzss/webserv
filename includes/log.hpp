/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:12:30 by okoca             #+#    #+#             */
/*   Updated: 2024/08/26 09:06:26 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef LOG_HPP
# define LOG_HPP

#include <webserv.hpp>

namespace http
{
	class Server;

	void	log(const std::string &s);

	void	log(const std::string &s, const Server &serv);

	void	log_err(const std::string &s);

	void	log_err(const std::string &s, const Server &serv);

	class Color
	{
	public:
		enum Code
		{
			RESET		= 0,
			FG_RED		= 31,
			FG_GREEN	= 32,
			FG_BLUE		= 34,
			FG_DEFAULT	= 39,
			BG_RED		= 41,
			BG_GREEN	= 42,
			BG_BLUE		= 44,
			BG_DEFAULT	= 49
		};
	public:
		static std::string set_color(Code code)
		{
			std::ostringstream s;

			s << "\033[" << code << "m";
			return s.str();
		}

		static std::string reset()
		{
			std::ostringstream s;

			s << "\033[" << Color::RESET << "m";
			return s.str();
		}
	};
}

#endif /* LOG_HPP */
