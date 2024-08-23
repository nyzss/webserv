#!/bin/bash

valgrind -q --track-fds=yes --leak-check=full --show-leak-kinds=all ./webserv