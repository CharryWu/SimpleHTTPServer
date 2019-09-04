/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

handTCPClient:
This module handles incoming connection, and message parsing and framing.
Handle error in data format level.

Data in valid format will be passed to responser for content verification and response
*******************************************************************************************/
#include "handleTCPClient.hpp"
using namespace std;

/****************************************
  Client Handler Threads for ThreadPool
**************************************/
void *HandleTCPClient(void *args){
      ThreadArgs *arg = static_cast<ThreadArgs *>(args);
      int servSock = arg->servSocket;
      string doc_root = arg->doc_root;
      unsigned int clntLen; /*Length of client address*/
      int clntSock; /*set client socket fd*/
      sockaddr_in echoClntAddr; /*Client IP address*/

      while(true){
  		/* Set the size of the in-out parameter */
  		clntLen = sizeof(echoClntAddr);

      /* clntSock is connected to a client! */
      cerr << "read for accept" << '\n';
  		if ((clntSock = accept(servSock, (sockaddr *) &echoClntAddr, &clntLen)) < 0){
  			DiewithMessage_t("accept() failed");
      }

      /* set socket receive timeout */
      struct timeval timeOutVal;
      timeOutVal.tv_sec = 5;
      timeOutVal.tv_usec = 0;
      if(setsockopt(clntSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeOutVal, sizeof(timeOutVal)) < 0){
                cerr << strerror(errno) << '\n';
        DiewithMessage("Called setsockopt(): socket option set failed"); /*socket creation failed*/
      }

      string addr(inet_ntoa(echoClntAddr.sin_addr));
  		cerr << "Handling client " + addr << '\n';

      /* Start Request Handling Process */
  		HandleReq(clntSock, doc_root);
    }
}

void HandleReq(int clntSock, string doc_root){
  /* P1: Framing and Parsing incoming bytes, parse into a request data structure */
  /* Handle timeout error */
  char buffer[BUFSIZE];
  memset(buffer, 0, sizeof(buffer));
  Framer framer;
  Parser parser;
  Responder responder(clntSock, doc_root);
  ssize_t numBytesRcvd;
  string msg;

  numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);
  if(numBytesRcvd < 0 && (errno == EWOULDBLOCK || errno == EAGAIN)){
    /* Insert handle timeout error response */
    cerr << "time out error" << '\n';
    responder.sendRespxonse(CLIENT_ERROR);
    close(clntSock);
    return;
  }


    while(numBytesRcvd > 0){
      string input(buffer, numBytesRcvd);
      framer.append(input);
      while(framer.hasMessage()){
          msg = framer.topMessage();
          framer.popMessage();
          // cerr << msg << '\n';
          if(!parser.parse(msg)){
            /* Insert handle invalid header error response */
            cerr << "header issue" << '\n';
            cerr << msg << '\n';
            responder.sendResponse(CLIENT_ERROR);
            close(clntSock);
            return;
          }

          /* Produce response based on header provided*/
          while(parser.isInstr()){
            cerr << "parse" << '\n';
            int status = responder.verifyandAppendReq(parser.getReqHeader());
            responder.sendResponse(status);
          }
          //
          // if(parser.isTerminated()){
          //   cout << "terminated" << '\n';
          //   break;
          // }

      }

      if(parser.isTerminated()){
        cout << "terminated" << '\n';
        break;
      }

      memset(buffer, 0, sizeof(buffer));
      numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);
      if(numBytesRcvd < 0 && (errno == EWOULDBLOCK || errno == EAGAIN)){
        /* Insert Handle timeout error response*/
        cerr << "time out" << '\n';
        responder.sendResponse(CLIENT_ERROR);
        close(clntSock);
        return;
      }
      cerr << numBytesRcvd << '\n';
    }

  /* Connection: close detected, close socket and return */
  cerr << "reach out of loop" << '\n';
  close(clntSock);
  cerr << "socket close" << '\n';
  return;
}
