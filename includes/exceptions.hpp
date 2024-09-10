/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:53:49 by okoca             #+#    #+#             */
/*   Updated: 2024/09/10 14:06:39 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

/*
# macro to generate exceptions
it takes a classname, and a `const char *` string.
*/
#define GENERATE_EXCEPTION(ClassName, ErrorMessage) \
	class ClassName : public std::exception \
	{ \
	public: \
		const char* what() const throw() { return ErrorMessage; } \
	};

#include <common.hpp>

namespace http
{
	class Exceptions
	{
	public:
		GENERATE_EXCEPTION(CGINotFound, "err: request CGI not found.")
		GENERATE_EXCEPTION(ExecutableNotFound, "err: cannot find executable.")
		GENERATE_EXCEPTION(Unhandled, "fatal: not handled yet.")
	};
}

#endif /* EXCEPTIONS_HPP */
