/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:12:19 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 12:06:40 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <log.hpp>

inline static std::string	log_data(const std::string &s);
inline static std::string	log_data(const std::string &s, const Server &serv);

void	log(const std::string &s)
{
	std::cout << log_data(s);
}

void	log(const std::string &s, const Server &serv)
{
	std::cout << log_data(s, serv);
}

void	log_err(const std::string &s)
{
	std::cerr << log_data(s);
}

void	log_err(const std::string &s, const Server &serv)
{
	std::cerr << log_data(s, serv);
}

inline static std::string	log_data(const std::string &s)
{
	std::ostringstream stream;
	std::time_t result = std::time(NULL);

	const char *t = std::asctime(std::localtime(&result));
	std::string	time_str(t + 11, 8);
	stream << "[" << time_str << "]";
	stream << " " << s;
	stream << std::endl;

	return stream.str();
}

inline static std::string	log_data(const std::string &s, const Server &serv)
{
	std::ostringstream stream;
	std::time_t result = std::time(NULL);

	const char *t = std::asctime(std::localtime(&result));
	std::string	time_str(t + 11, 8);
	stream << "[" << time_str << "]";
	stream << " - [" << serv.get_address() << "] ";
	stream << s;
	stream << std::endl;

	return stream.str();
}
