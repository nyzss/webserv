/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:05:08 by okoca             #+#    #+#             */
/*   Updated: 2024/08/22 14:22:19 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <common.hpp>
#include <request.hpp>

class Response
{
private:
	SOCKET	_fd;
	Request	_req;

public:
	Response ();
	Response (const Response &val);
	~Response ();
	Response & operator=(const Response &val);

public:
	void send();

public:
	Response (const Request &req);
};

#endif /* RESPONSE_HPP */
