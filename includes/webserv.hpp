/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:53:26 by okoca             #+#    #+#             */
/*   Updated: 2024/08/25 14:56:25 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "common.hpp"

#include "socket.hpp"
#include "log.hpp"
#include "cluster.hpp"
#include "server.hpp"
#include "client.hpp"
#include "request.hpp"
#include "response.hpp"
#include "dumb_ptr.hpp"

#include "defaults.hpp"

#include "utils.hpp"

# define UPLOADED "example/upload/"

extern bool	end_sig;

extern char **environ;

#endif /* WEBSERV_HPP */
