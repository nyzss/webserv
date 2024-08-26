/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 08:56:44 by okoca             #+#    #+#             */
/*   Updated: 2024/08/26 11:02:33 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <webserv.hpp>

namespace http
{
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

	bool	is_dir(const std::string &s)
	{
		return is_dir(s.c_str());
	}

	bool	is_dir(const char *s)
	{
		struct stat	path_stat;

		if (stat(s, &path_stat) != 0)
			return false;
		return S_ISDIR(path_stat.st_mode);
	}

	bool	is_file(const std::string &s)
	{
		return is_file(s.c_str());
	}

	bool	is_file(const char *s)
	{
		struct stat	path_stat;

		if (stat(s, &path_stat) != 0)
			return false;
		return S_ISREG(path_stat.st_mode);
	}

	std::string	get_extension(const std::string &path)
	{
		size_t pos = path.find_last_of('.');
		if (pos == std::string::npos)
			return path;

		std::string	ext = path.substr(pos + 1);
		return ext;
	}


}