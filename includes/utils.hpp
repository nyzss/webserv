/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:36:08 by okoca             #+#    #+#             */
/*   Updated: 2024/08/22 14:38:07 by okoca            ###   ########.fr       */
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

#endif /* UTILS_HPP */
