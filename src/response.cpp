/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:06:21 by okoca             #+#    #+#             */
/*   Updated: 2024/08/22 15:47:39by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

Response::Response(Request &req): _req(req)
{
	_resource_exists = false;
	_prefix = "example";
	_cgi = false;
	_fd = -1;
}

void Response::send()
{
	this->_fd = _req.get_sockfd();
	if (_fd < 0)
		throw std::runtime_error("response object initialised with invalid fd");

	check_cgi();
	if (!_cgi)
	{
		init_resource();
		builder();
	}
	write();
}

void Response::cgi_handler(const std::string &cgi)
{
	char	*args[3];
	int		fd[2];

	if (pipe(fd) != 0)
		throw std::runtime_error("failed to initialise pipes for cgi");
	pid_t pid = fork();
	if (pid < 0)
		throw std::runtime_error("failed to fork process for cgi");
	if (pid == 0)
	{
		std::string	exec = "/usr/bin/" + cgi;
		std::string file = _prefix + _req.get_path();

		args[0] = const_cast<char *>(exec.c_str());
		args[1] = const_cast<char*>(file.c_str());
		args[2] = NULL;
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		execve(args[0], args, environ);
		throw std::runtime_error("execve failed, exit.");
	}
	close(fd[1]);

	std::string	cgi_buffer;

	char buf[DEFAULT_READ];
	int	b_read;
	while ((b_read = read(fd[0], buf, 1024)))
		cgi_buffer.append(buf, b_read);
	close(fd[0]);

	std::string body;
	size_t	cgi_header_end = cgi_buffer.find("\r\n\r\n");
	if (cgi_header_end != std::string::npos)
		body = cgi_buffer.substr(cgi_header_end + 4);

	std::string len = Defaults::Fields()[CONTENT_LENGTH];
	len += to_string(body.size());
	len += "\n";
	cgi_buffer.insert(0, len);

	std::string t = Defaults::Fields()[VERSION];
	t += Defaults::Status()[OK];
	t += "\n";
	cgi_buffer.insert(0, t);

	std::cout << "buffer: \n" << cgi_buffer << std::endl;

	_final.insert(_final.end(), cgi_buffer.begin(), cgi_buffer.end());
}

void	Response::check_cgi()
{
	std::string	ext = get_extension(_req.get_path());
	if (ext == "py")
	{
		_cgi = true;
		cgi_handler("python3");
	}
}

void Response::end_line()
{
	_buffer += "\r\n";
}

void Response::add_line(const std::string &line)
{
	_buffer += line;
	end_line();
}

std::string Response::init_content_type() const
{
	std::string content_type = "Content-Type: ";

	std::string	ext = get_extension(_req.get_path());
	if (!_resource_exists || ext == _req.get_path())
	{
		content_type += "text/html";
		return content_type;
	}

	if (ext == "html")
		content_type += "text/html";
	else if (ext == "css")
		content_type += "text/css";
	else if (ext == "js")
		content_type += "application/javascript";
	else if (ext == "jpg")
		content_type += "image/jpeg";
	else if (ext == "png")
		content_type += "image/png";
	else if (ext == "gif")
		content_type += "image/gif";
	else
		content_type += "text/html";
	return content_type;
}

std::string	Response::init_status_line() const
{
	std::string status_line = Defaults::Fields()[VERSION];
	if (!_resource_exists)
		status_line += Defaults::Status()[NOT_FOUND];
	else
		status_line += Defaults::Status()[OK];
	return status_line;
}

std::string	Response::init_content_len() const
{
	std::string content_len = Defaults::Fields()[CONTENT_LENGTH];
	content_len += to_string(_raw_size);
	return content_len;
}

std::string	Response::init_connection() const
{
	std::string connection = Defaults::Fields()[CONNECTION];
	connection += "close";
	return connection;
}

void Response::combine()
{
	_final.reserve(_buffer.size() + _raw_size);
	_final.insert(_final.end(), _buffer.begin(), _buffer.end());
	_final.insert(_final.end(), _raw_data.begin(), _raw_data.end());
}

void Response::builder()
{
	add_line(init_status_line());
	add_line(init_content_type());
	add_line(init_content_len());
	add_line(init_connection());
	end_line();

	combine();
}

void Response::write()
{
	int r_sd = ::send(_fd, _final.data(), _final.size(), 0);
	if (r_sd < 0)
		throw std::runtime_error("client couldn't communicate with server!");
}

void Response::read_file(std::ifstream &file)
{
	if (!_resource_exists || !file)
	{
		_raw_data.insert(_raw_data.end(), Defaults::html_not_found.begin(), Defaults::html_not_found.end());
		_raw_size = _raw_data.size();
		return ;
	}
	_raw_size = file.tellg();
	file.seekg(0, std::ios::beg);
	_raw_data.resize(_raw_size);

	if (!file.read(reinterpret_cast<char *>(_raw_data.data()), _raw_size))
		throw std::runtime_error("failed to read file!");
}

void Response::init_resource()
{
	std::string filename = _prefix;
	std::ifstream	file;

	filename += _req.get_path();

	file.open(filename.c_str(), std::ios::binary | std::ios::ate);
	if (file.good())
		_resource_exists = is_file(filename);
	read_file(file);
}

void Response::debug() const
{
	std::string	partial(_raw_data.begin(), _raw_data.end());
	std::cout << "---------------\nRESPONSE HEADER: \n" << _buffer << "\n";

	std::cout << "---------------\nRESPONSE BODY: \n";
	if (_raw_size > 100)
		std::cout << "[OMITTED]" << std::endl;
	else
		std::cout << partial << std::endl;
}

Response & Response::operator=(const Response &val)
{
	if (this != &val)
	{
		this->_fd = val._fd;
		this->_req = val._req;
		this->_buffer = val._buffer;

		this->_prefix = val._prefix;
		this->_resource_exists = val._resource_exists;

		this->_raw_data = val._raw_data;
		this->_raw_size = val._raw_size;
		this->_final = val._final;
	}
	return *this;
}

Response::~Response()
{}

Response::Response(const Response &val) : _req(val._req)
{
	*this = val;
}
