/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

httpd.cpp:
Concurrency:
implementation of server start up, creating thread pool.
*******************************************************************************************/
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <cstring>
#include <string>
#include "httpd.h"
#include "diewithmessage.hpp"
#include "handleTCPClient.hpp"

using namespace std;

void start_httpd(unsigned short port, string doc_root)
{
	int servSocket; /*set server socket fd*/
	const string root = doc_root; /*Set the document serving root*/
	sockaddr_in echoServAddr; /*Local IP address*/
	sockaddr *echoServAddr_t;
	unsigned short echoServPort = port; /*set server port*/
	pthread_t pid[POOL_SIZE]; /*Initiate threads id for pool*/
	ThreadArgs *args = new ThreadArgs;

	cerr << "Starting server (port: " << port <<
		", doc_root: " << doc_root << ")" << endl;

	if((servSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		DiewithMessage("Called socket(): Socket create failed"); /*socket creation failed*/
	}

	/* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

	echoServAddr_t = (sockaddr *)&echoServAddr;
	if(bind(servSocket, echoServAddr_t, sizeof(echoServAddr)) < 0){
		DiewithMessage("Called bind(): socket binding failed"); /*bind socket failed*/
	}

	if(listen(servSocket, CONNECTION_SIZE) < 0){ /*CONNECTION_SIZE can be found in server_utils*/
		DiewithMessage("Called listen(): listen failed"); /*listen on socket failed*/
	}

	/*Initiate a thread pool, specified by POOL_SIZE*/
	args->servSocket = servSocket;
	args->doc_root = doc_root;
	for(unsigned i = 0; i < POOL_SIZE; i++){
		//create running process
		if(pthread_create(&pid[i], NULL, &HandleTCPClient, args) < 0){
			DiewithMessage("Called pthread_create(): threads creation failed");
		}
		cerr << "Thread " << pid[i] << "is spawned \n";
	}

	cerr << "Starting server (port: " << port <<
		", doc_root: " << doc_root << ")" << endl;

	for(unsigned i = 0; i < POOL_SIZE; i++){
		/*Block main process until threads join*/
		if(pthread_join(pid[i], NULL) < 0){
			DiewithMessage("Called pthread_join(): threads join failed");
		}
	}
	/*Not reachable*/
}
