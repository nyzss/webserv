#!/usr/bin/env python3

import cgi
import sys

# sys.stdout.write("Content-Type: text/html\r\n\r\n")


print("Content-Type: text/html\n")

print("<html>")
print("<head>")
print("<title>Simple Python CGI</title>")
print("</head>")
print("<body>")
print("<h1>Hello, World!</h1>")
print("<p>This is a simple Python CGI script.</p>")
print("</body>")
print("</html>")