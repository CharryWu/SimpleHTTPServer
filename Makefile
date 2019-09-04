
CC=g++
CFLAGS=-ggdb -std=c++11 -Wall -Wextra -pedantic
DEPS = httpd.h diewithmessage.hpp framer.hpp handleTCPClient.hpp parser.hpp responder.hpp
SRCS = httpd.cpp handleTCPClient.cpp diewithmessage.cpp framer.cpp parser.cpp responder.cpp
MAIN_SRCS = main.c $(SRCS)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)

default: httpd

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

httpd:    $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o httpd $(MAIN_OBJS) -lpthread

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f httpd *.o
