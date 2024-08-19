/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:16:58 by okoca             #+#    #+#             */
/*   Updated: 2024/08/19 14:20:23 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <common.hpp>

class Request
{
public:
	enum method
	{
		GET,
		POST,
		DELETE,
		LAST = DELETE
	};
private:
	method	_method;
	std::string	_path;
	static const char *methods[];
	Request ();

public:
	Request (const std::string &req);
	Request (const Request &val);
	~Request ();
	Request & operator=(const Request &val);
};

#endif /* REQUEST_HPP */
