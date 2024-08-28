/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:53:09 by okoca             #+#    #+#             */
/*   Updated: 2024/08/28 12:21:12 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defaults.hpp"
#include "parser.hpp"
#include "webserv.hpp"

void	handle_sigint(int sig)
{
	(void)sig;
	throw std::runtime_error("exiting...");
}

int main()
{
	http::Cluster	cluster;

	try
	{
		signal(SIGINT, handle_sigint);
		cluster.add_server(new http::Server("0.0.0.0", 8080));
		cluster.add_server(new http::Server("0.0.0.0", 8081));
		cluster.add_server(new http::Server("0.0.0.0", 8082));
		cluster.add_server(new http::Server("0.0.0.0", 8083));
		cluster.add_server(new http::Server("0.0.0.0", 8084));

		cluster.start();
	}
	catch(const std::exception& e)
	{
		http::log_err(e.what());
	}
}
