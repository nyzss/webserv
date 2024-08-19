/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 08:56:44 by okoca             #+#    #+#             */
/*   Updated: 2024/08/19 09:05:00 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

std::vector<std::string>	ws_split(std::string s, char delim)
{
	std::vector<std::string> vec;
	size_t	pos = 0;
	std::string	tok;

	while ((pos = s.find(delim)) != std::string::npos)
	{
		tok = s.substr(0, pos);
		s.erase(0, pos + 1);
		vec.push_back(tok);
	}
	vec.push_back(s);

	return vec;
}

std::vector<std::string>	ws_split(std::string s, const std::string &delim)
{
	std::vector<std::string> vec;
	size_t	pos = 0;
	std::string	tok;

	while ((pos = s.find(delim)) != std::string::npos)
	{
		tok = s.substr(0, pos);
		s.erase(0, pos + delim.length());
		vec.push_back(tok);
	}
	vec.push_back(s);

	return vec;
}