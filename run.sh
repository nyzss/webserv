#!/bin/bash

# valgrind -q --track-fds=yes --leak-check=full --show-leak-kinds=all ./webserv
if [ "$1" == "q" ]; then
    valgrind -q --track-fds=yes --leak-check=full --show-leak-kinds=all ./webserv
else
    valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all ./webserv
fi