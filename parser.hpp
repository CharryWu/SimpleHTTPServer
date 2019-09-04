/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

Parser.hpp: header file
Verify the header format and parse the request into valid request data structure
*******************************************************************************************/


#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <queue>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#include "httpd.h"
using namespace std;

typedef struct HttpInstruction_t {
	// DEFINE YOUR DATA STRUCTURE HERE
	/* Noted: the only correct method is GET, therefore no instruction is indicated here*/
	string url;
	float http_ver;
	string host;
	string connection;
} HttpInstruction;

class Parser {
	private:
		queue<HttpInstruction> reqQ;
		HttpInstruction _req;
		bool hasInstr;
		bool _isTerminated;
		vector<string> parseHelper(string insstr, char del);
	public:
		/*
			Constructor, initialize private fields
		*/
		Parser();

		/*
			Parse string into valid header
			Return: true, if string contains valid header; otherwise false
		*/
		bool parse(string insstr);

		/*
			Return currently parsed http headers
		*/
		HttpInstruction getReqHeader(){
			if(!reqQ.empty()){
				HttpInstruction res = reqQ.front();
				reqQ.pop();
				return res;
			}
			return _req;
		}

		/*
			Return if connection needs to be terminated by client
		*/
		bool isTerminated(){
			return _isTerminated;
		}

		bool isInstr(){
			return !reqQ.empty();
		}
};

#endif // CALCPARSER_HPP
