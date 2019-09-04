/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

httpd.hpp:
Concurrency: header file
implementation of server start up, creating thread pool.
*******************************************************************************************/

#ifndef HTTPD_H
#define HTTPD_H

const unsigned CONNECTION_SIZE = 20;
const unsigned POOL_SIZE = 10;
const float HTTP_VER_UPPER = 1.1;
const std::string SERVER_VER_NAME = "Baba/v1.0-dev";

struct ThreadArgs{
	int servSocket;
	std::string doc_root;
};

void start_httpd(unsigned short port, std::string doc_root);

#endif // HTTPD_H
