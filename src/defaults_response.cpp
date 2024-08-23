/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaults_response.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 09:10:41 by okoca             #+#    #+#             */
/*   Updated: 2024/08/23 10:01:03 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

const std::string	Response::HTML_NOT_FOUND =	"<html><body>"
										"<h1>404 - Not Found</h1>"
										"</html></body>";

inline static std::map<STATUS_CODE, const char *> generate_status()
{
	std::map<STATUS_CODE, const char *> map;

	map[OK] = "200 OK";
	map[NOT_FOUND] = "404 Not Found";
	return map;
}

const std::map<STATUS_CODE, const char *> Response::STATUS = generate_status();