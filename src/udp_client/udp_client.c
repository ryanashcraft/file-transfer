
/**
 * udp_client.c
 * 
 * Simple client for sending data over UDP.
 *
 * Required arguments: input filename, IP address of server to connect to, port on which
 * to connect.
 * 
 * @author Ryan Ashcraft
 */

#include <stdio.h>
#include <stdlib.h>

#include "socket.h"

#define BUFFER_SIZE 512
#define LOG_PRE "[udpclient] "

int main(int argc, char** argv) {
	if (argc != 4) {
		fprintf(stderr, "%s\n", "required arguments: input file, IP address, port");
		exit(-1);
	}

	sock_desc sock = sock_udp_init();
	sock_connect(sock, argv[2], atoi(argv[3]));

	FILE* input = fopen(argv[1], "r");
	char* buffer = malloc(BUFFER_SIZE);

	size_t size_read = 0;
	while ((size_read = fread(buffer, sizeof(char), BUFFER_SIZE, input))) {
		sock_send(sock, size_read, buffer);
	}

	fprintf(stderr, LOG_PRE "%s\n", "file sent, closing socket");

	sock_close(sock);

	free(buffer);
	fclose(input);
}
