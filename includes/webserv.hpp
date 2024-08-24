/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:53:26 by okoca             #+#    #+#             */
/*   Updated: 2024/08/24 12:06:49 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <common.hpp>

#include "socket.hpp"
#include "log.hpp"
#include "server.hpp"
#include "client.hpp"
#include "request.hpp"
#include "response.hpp"
#include "dumb_ptr.hpp"

#include "defaults.hpp"

#include "utils.hpp"

# define UPLOADED "./upload"

extern bool	end_sig;

#endif /* WEBSERV_HPP */
