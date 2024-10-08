/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:36:08 by okoca             #+#    #+#             */
/*   Updated: 2024/09/09 14:50:00 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef UTILS_HPP
# define UTILS_HPP

# include <common.hpp>

namespace http
{
	std::vector<std::string>	ws_split(std::string s, char delim);
	std::vector<std::string>	ws_split(std::string s, const std::string &delim);

	template <typename T>
	std::string	to_string(const T& val)
	{
		std::ostringstream	s;

		s << val;
		return s.str();
	}

	bool	is_dir(const char *s);
	bool	is_dir(const std::string &s);

	bool	is_file(const char *s);
	bool	is_file(const std::string &s);

	std::string	get_extension(const std::string &path);

	void	set_non_blocking(FD fd);

	void	brute_close(FD &fd);
}

#endif /* UTILS_HPP */
