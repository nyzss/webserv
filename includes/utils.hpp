/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:36:08 by okoca             #+#    #+#             */
/*   Updated: 2024/08/25 21:23:29 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef UTILS_HPP
# define UTILS_HPP

# include <common.hpp>

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

size_t		get_header_end(const std::string &s);

#endif /* UTILS_HPP */
