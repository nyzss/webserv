/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:12:19 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 11:35:14 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <log.hpp>

// could also make a function that takes in a server and outputs that too

void	log(const std::string &s)
{
	std::ostringstream stream;
	std::time_t result = std::time(NULL);

	const char *t = std::asctime(std::localtime(&result));
	std::string	time_str(t + 11, 8);
	stream << "[" << time_str << "]";
	stream << " " << s;
	stream << std::endl;

	std::cout << stream.str();
}