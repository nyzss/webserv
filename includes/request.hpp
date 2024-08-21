/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:16:58 by okoca             #+#    #+#             */
/*   Updated: 2024/08/21 18:00:37 by okoca            ###   ########.fr       */
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
	static const char *_methods_arr[];
	size_t	_needed_size;
	size_t	_current_size;

public:
	Request ();
	Request (const std::string &req);
	Request (const Request &val);
	~Request ();
	Request & operator=(const Request &val);
	std::string	get_path() const;
	method get_method() const;
	std::string get_method_str() const;
};

#endif /* REQUEST_HPP */
