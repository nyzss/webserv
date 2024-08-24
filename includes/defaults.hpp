/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaults.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:25:10 by okoca             #+#    #+#             */
/*   Updated: 2024/08/24 12:51:36 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef DEFAULTS_HPP
# define DEFAULTS_HPP

#include <common.hpp>

enum status_code
{
	OK,
	NOT_FOUND,
	STATUS_COUNT,
};

enum header_field
{
	VERSION,
	CONTENT_TYPE,
	CONTENT_LENGTH,
	CONNECTION,
	FIELD_COUNT,
};

enum content_type
{
	FORMDATA,
	OCTEC_STREAM,
	IMAGE_JPEG,
	IMAGE_PNG,
	IMAGE_WEBP,
	IMAGE_GIF,
	CONTENT_TYPE_COUNT,
};

class Defaults
{
private:
	static const char *_status[STATUS_COUNT];
	static const char *_fields[FIELD_COUNT];
	static const char *_content_type[CONTENT_TYPE_COUNT];

public:
	static const std::string	html_not_found;

public:
	class	Fields
	{
	public:
		const char *operator[](size_t field) const
		{
			if (field >= FIELD_COUNT)
				throw std::out_of_range("trying to access field that is out of range");
			return Defaults::_fields[field];
		}
	};

	class	Status
	{
	public:
		const char *operator[](size_t code) const
		{
			if (code >= STATUS_COUNT)
				throw std::out_of_range("trying to access status code that is out of range");
			return Defaults::_status[code];
		}
	};

	class	ContentType
	{
	public:
		const char *operator[](size_t type) const
		{
			if (type >= CONTENT_TYPE_COUNT)
				throw std::out_of_range("trying to access status code that is out of range");
			return Defaults::_content_type[type];
		}
	};

public:
	virtual ~Defaults() = 0;
};

#endif /* DEFAULTS_HPP */
