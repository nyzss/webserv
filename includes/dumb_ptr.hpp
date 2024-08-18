/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dumb_ptr.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 16:15:34 by okoca             #+#    #+#             */
/*   Updated: 2024/08/18 16:36:41 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef DUMB_PTR_HPP
# define DUMB_PTR_HPP


template <typename T>
class dumb_ptr
{
private:
	T	*_data;

public:
	dumb_ptr (): _data(NULL) {}
	dumb_ptr (T val)
	{
		this->_data = new T(val);
	}
	dumb_ptr (T *val)
	{
		this->_data = val;
	}
	dumb_ptr (const dumb_ptr &val) : _data()
	{
		this->_data = new T(*val._data);
	}
	~dumb_ptr ()
	{
		if (this->_data != NULL)
			delete this->_data;
		this->_data = NULL;
	}
	dumb_ptr & operator=(const dumb_ptr &val)
	{
		if (this != &val)
		{
			this->_data = new T(*val._data);
		}
		return *this;
	}
	dumb_ptr & operator=(const T &val)
	{
		this->_data = new T(val);
		return *this;
	}

	T	&operator*()
	{
		return *this->_data;
	}

	T	&operator->()
	{
		return *(this->_data);
	}
};

#endif /* DUMB_PTR_HPP */
