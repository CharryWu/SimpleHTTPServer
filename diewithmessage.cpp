/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

diewithmessage.cpp:
System util:
end server if critical error is detected
*******************************************************************************************/
#include "diewithmessage.hpp"

void DiewithMessage(std::string msg){
  std::cerr << msg << '\n';
  exit(1);
}

void DiewithMessage_t(std::string msg){
  std::cerr << msg <<'\n';
  pthread_exit(NULL);
}
