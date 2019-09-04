# cse291-project1: Triton HttpServer

Specification: https://cseweb.ucsd.edu/~gmporter/classes/sp18/cse291/post/project1/

## To build

1. Fork code
2. make
3. usage: ./httpd [port number] [htdocs root]
4. You may use `curl -v localhost:[port number]` to send http request to server

### Design:
1. Http framer
2. Http parser
3. Http responder

You may extend the server by creating middle ware to handle received Http request.
Request is parsed into HttpInstruction (found in parser.cpp)

## Author
Name: Hou Wang
All rights reserved 

