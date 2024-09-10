/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:19:18 by okoca             #+#    #+#             */
/*   Updated: 2024/09/09 13:46:37 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef COMMON_HPP
# define COMMON_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <set>
#include <limits>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <cerrno>
#include <cstdio>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <ctime>

typedef int	SOCKET;
typedef int	PIPE;
typedef int	FD;
typedef int	PORT;

#endif /* COMMON_HPP */
