/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

responder.cpp
Use data flow model, responder handle file checks, permission verification and response message formatting

handle reponse based on request data structure
P2: Implement basic URL check policy, avoid client from accessing unauthorized path
Check host
P3: response with ERROR, response, response with body
*******************************************************************************************/

#ifndef RESPONDER_HPP
#define RESPONDER_HPP

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>
#include <sys/sendfile.h>
#include "httpd.h"
#include "parser.hpp"
using namespace std;

const string ERRORPAGE = "./errorPage";
const int FORBIDDEN = 403;
const string FORBIDDEN_DES = "Forbidden";
const string FORBIDDEN_PATH = ERRORPAGE + "/Forbidden.html";
const int NOT_FOUND = 404;
const string NOT_FOUND_DES = "Not Found";
const string NOT_FOUND_PATH = ERRORPAGE + "/NotFound.html";
const int CLIENT_ERROR = 400;
const string CLIENT_ERROR_DES = "Bad Request";
const string CLIENT_ERROR_PATH = ERRORPAGE + "/ClientError.html";
const int OK = 200;
const string OK_DES = "OK";
const string DELIMITER = "\r\n\0";
const string HTTP = "HTTP/";

const string CONTENT_TYPE = "Content-Type: ";
const string CONTENT_LEN = "Content-Length: ";
const string LAST_MOD = "Last-Modified: ";
const string SERVER = "Server: ";
const string CONTENT_TXT = "text/html";
const string CONTENT_JPEG = "image/jpeg";
const string CONTENT_PNG = "image/png";

enum FileType{
  TEXT,
  JPEG,
  PNG
};

class Responder {
  private:
    string doc_root;
    int clntSock;
    int fd;
    FileType type;
    string sendQ;

    vector<string> parseHelper(string insstr, char del);
    /*
      send out file described by fd, in statCode
    */
    void response(int statCode);
    /*
      verify request header and generate status code
    */
    int verifyReq(HttpInstruction req);

    /*
      check file permission, existence
      if not problem set this->fd to the file
    */
    int checkFile(string path);

    /*
      check and set file type.
      If required file is not supported, error status code returns
      otherwise return 0
    */
    int setFileType(string path);

    /*
      Helpers for Appending headers
    */
    void appendInitLine(int statCode);
    void appendContentType();
    void appendContentLength();
    void appendLastModified();
    void appendServ(string serv);

  public:
  	/*
  		Constructor, initialize private fields
      Initialized with client socket
  	*/
  	Responder(int client, string root){
        clntSock = client;
        doc_root = root;
        this->sendQ = "";
    };

    /*
      verify if request is valid.
      Return verified status code, can be passed to sendResponse
    */
    int verifyandAppendReq(HttpInstruction req);

    /*
      send response based on verified req,
      if status code is error (2xx), then no request is needed to be apend
      return true if response is sent
      else false if response not sent
    */
    void sendResponse(int statCode);

};

#endif // CALCPARSER_HPP
