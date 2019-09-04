/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

Parser.cpp
Verify the header format and parse the request into valid request data structure
*******************************************************************************************/

#include "parser.hpp"

using namespace std;
/* Constructor */
Parser::Parser(){
	  hasInstr = false;
		_isTerminated = false;
}

vector<string> Parser::parseHelper(string insstr, char del){
	vector<string> res;
	unsigned begin = 0;
	unsigned end = 0;

	while(end < insstr.size()){
		if(insstr[end] == del){
			res.push_back(insstr.substr(begin, end - begin));
			begin = end + 1;
		}
			end++;
	}

	if(begin != end){
		res.push_back(insstr.substr(begin, end - begin));
	}
	return res;
}

// need to modify parser to parse header
bool Parser::parse(string insstr){
	// PUT YOUR CODE HERE
	if(insstr.compare("") == 0){
		// delimiter
		reqQ.push(_req);
		return true;
	}

	vector<string> key_val;
	key_val = parseHelper(insstr, ' ');
	cerr << insstr << '\n';
	cerr << key_val.size() << '\n';
	if(key_val.size() == 3){
		/*
			This is the http initial line:
			1). verify if GET is correct
			2). verify if http version is supported
		*/

		if(key_val[0].compare("GET") != 0){
			cerr << "not GET" << '\n';
			return false;
		}

		/* Path overwrite for "/" default to index.html*/
		_req.url = key_val[1];
		if(_req.url == "/"){
			_req.url = "/index.html";
		}
		// cerr << "URL: " <<key_val[1];

		char *http_t = strtok(static_cast<char *>(&key_val[2][0]), "/");
		string http(http_t);
		if(http.compare("HTTP") != 0){
			// if HTTP version is not correctly formed
			cerr << "not HTTP" << '\n';
			return false;
		}
		_req.http_ver = stof(strtok(NULL, "/"));

		/*
			verify http version is supported
		*/
		if(_req.http_ver > HTTP_VER_UPPER){
			cerr << "HTTP version not supported" << '\n';
			return false;
		}
	}else if(key_val.size() == 2){
		/*
			This is the "key: val" pair
			1). verify if this header is supported
		*/
		string key = key_val[0];
		string val = key_val[1];
		// cerr << key << '\n';
		// cerr << val << '\n';
		if(key.compare("Host:") == 0){
			_req.host = val;
		}else if(key.compare("Connection:") == 0){
			_req.connection = val;
			if(val.compare("close") == 0){
				_isTerminated = true;
			}
		}else{
			/* No column*/
			if(key.find(":") == string::npos){
				return false;
			}
		}
	}else{
		/* Headers malform*/
		return false;
	}

	return true;
}
