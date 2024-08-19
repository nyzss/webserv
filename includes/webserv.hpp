/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:53:26 by okoca             #+#    #+#             */
/*   Updated: 2024/08/19 09:05:16 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <common.hpp>

#include "socket.hpp"
#include "log.hpp"
#include "server.hpp"
#include "dumb_ptr.hpp"

std::vector<std::string>	ws_split(std::string s, char delim);
std::vector<std::string>	ws_split(std::string s, const std::string &delim);

extern bool	end_sig;

#endif /* WEBSERV_HPP */
