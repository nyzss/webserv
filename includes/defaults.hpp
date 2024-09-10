/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaults.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:25:10 by okoca             #+#    #+#             */
/*   Updated: 2024/09/10 11:10:48 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef DEFAULTS_HPP
# define DEFAULTS_HPP

#include <common.hpp>

namespace http
{
	struct	StatusCode
	{
		enum Value
		{
			OK,
			NOT_FOUND,
			INTERNAL_SERVER_ERROR,
			COUNT
		};
	};

	struct HeaderField
	{
		enum Value
		{
			VERSION,
			CONTENT_TYPE,
			CONTENT_LENGTH,
			CONNECTION,
			COUNT,
		};
	};

	struct ContentType
	{
		enum Value
		{
			HTML,
			CSS,
			JAVASCRIPT,
			FORMDATA,
			OCTEC_STREAM,
			IMAGE_JPEG,
			IMAGE_PNG,
			IMAGE_WEBP,
			IMAGE_GIF,
			COUNT,
		};
	};

	class Defaults
	{
	private:
		static const char *_status[StatusCode::COUNT];
		static const char *_fields[HeaderField::COUNT];
		static const char *_content_type[ContentType::COUNT];

	public:
		static const std::string	html_not_found;
		static const std::string	html_internal_server_error;

	public:
		static const char	*get_status_code(StatusCode::Value code);
		static const char	*get_header_field(HeaderField::Value field);
		static const char	*get_content_type(ContentType::Value type);
	};
}

#endif /* DEFAULTS_HPP */
