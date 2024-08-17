/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:12:30 by okoca             #+#    #+#             */
/*   Updated: 2024/08/17 11:48:29 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef LOG_HPP
# define LOG_HPP

#include <webserv.hpp>

class Server;

void	log(const std::string &s);

void	log(const std::string &s, const Server &serv);

#endif /* LOG_HPP */
