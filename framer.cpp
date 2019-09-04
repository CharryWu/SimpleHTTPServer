/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

framer.cpp:
framing incoming bytes stream into http packets.
*******************************************************************************************/
#include "framer.hpp"
using namespace std;

/***************************
	Public Methods
*******************************/
void Framer::append(string chars)
{
	// PUT YOUR CODE HERE
	for(unsigned i = 0; i < chars.length(); i++){
		_msgBuffer.push_back(chars[i]);
	}
	return;
}

bool Framer::hasMessage() const
{
	// PUT YOUR CODE HERE
	if(_msgBuffer.size() == 0){
		return false;
	}

	for(auto be = Framer::_msgBuffer.begin(), en = Framer::_msgBuffer.end(); be != en; ++be){
		char cur = *be;
		if(cur == Framer::CR && be != en){
			char next = *(be + 1);
			if(next == Framer::LF){
					return true;
			}
		}
	}
	return false;
}

string Framer::topMessage() const
{
	// PUT YOUR CODE HERE
	string msg;
	for(auto be = _msgBuffer.begin(), en = _msgBuffer.end(); be != en; ++be){
		char cur = *be;
		if(cur == Framer::CR && be != en){
			char next = *(be + 1);
			if(next == Framer::LF){
					break;
				}
			}

			if(cur != Framer::CR && cur != Framer::LF){
					msg += cur;
			}
		}
	return msg;
}


void Framer::popMessage()
{
	// PUT YOUR CODE HERE
	for(auto be = _msgBuffer.begin(), en = _msgBuffer.end(); be != en; ++be){
		char cur = *be;
		if(cur == Framer::CR && be != en){
			char next = *(be + 1);
			if(next == Framer::LF){
				if(be + 1 == en){
					_msgBuffer.erase(_msgBuffer.begin(), _msgBuffer.end());
				}else{
					advance(be, 2);
					_msgBuffer.erase(_msgBuffer.begin(), be);
				}
				return;
			}
		}
	}

	return;
}

void Framer::printToStream(ostream& stream) const
{
	// (OPTIONAL) PUT YOUR CODE HERE--useful for debugging
	string msg(Framer::_msgBuffer.begin(), Framer::_msgBuffer.end());
	stream << msg << '\n';

}
