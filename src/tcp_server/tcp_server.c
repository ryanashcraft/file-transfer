
/**
 * tcp_server.c
 * 
 * Simple server for sending data over TCP.
 *
 * Required arguments: output filename, IP address to bind to, port to bind to
 * 
 * @author Ryan Ashcraft
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "socket.h"

#define MAX_REQUESTS 5
#define BUFFER_SIZE 512
#define LOG_PRE "[tcpserver] "

int main(int argc, char** argv) {
	if (argc != 4) {
		fprintf(stderr, "%s\n", "required arguments: output file, IP address, port");
		exit(-1);
	}

	sock_desc isocket = sock_tcp_init();
	sock_bind(isocket, argv[2], atoi(argv[3]));
	sock_listen(isocket, MAX_REQUESTS);
	
	FILE* output = fopen(argv[1], "w");
	char* buffer = malloc(BUFFER_SIZE);
	assert(buffer);

	sock_desc csocket = sock_accept(isocket);

	int message_size = 0;

	/* Receive all data from client socket, then proceed to close connection */
	while ((message_size = sock_receive(csocket, BUFFER_SIZE, buffer))) {
		buffer[message_size] = '\0';
		fwrite(buffer, sizeof(char), message_size, output);
		fflush(output);
	}

	fprintf(stderr, LOG_PRE "data received and written to %s\n", argv[1]);
	fprintf(stderr, LOG_PRE "%s\n", "closing sockets");

	sock_close(csocket);
	sock_close(isocket);

	free(buffer);
	fclose(output);

	return 0;
}
