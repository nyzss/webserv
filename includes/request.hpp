/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:16:58 by okoca             #+#    #+#             */
/*   Updated: 2024/08/19 09:29:58 by okoca            ###   ########.fr       */
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
		NONE,
		GET,
		POST,
		DELETE
	};
private:
	method	_method;
	std::string	_path;

public:
	Request ();
	Request (const std::string &req);
	Request (const Request &val);
	~Request ();
	Request & operator=(const Request &val);
};

#endif /* REQUEST_HPP */
