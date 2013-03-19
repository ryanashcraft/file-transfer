
/**
 * udp_client.c
 * 
 * Simple server for sending data over UDP.
 *
 * Required arguments: output filename, IP address to bind to, port to bind to
 * 
 * @author Ryan Ashcraft
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>

#include "socket.h"

#define MAX_REQUESTS 5
#define BUFFER_SIZE 512
#define LOG_PRE "[udpserver] "

static sock_desc isocket;
static char* buffer;
static FILE* output;

void exit_handler(int signal) {
	sock_close(isocket);

	free(buffer);
	fclose(output);

	exit(1);
}

void listen_to_signal(int signal, void(*handler)(int)) {
	struct sigaction sig_action;

	sig_action.sa_handler = handler;
	sigemptyset(&sig_action.sa_mask);
	sig_action.sa_flags = 0;

	sigaction(signal, &sig_action, NULL);
}

int main(int argc, char** argv) {
	if (argc != 4) {
		fprintf(stderr, "%s\n", "required arguments: output file, IP address, port");
		exit(-1);
	}

	listen_to_signal(SIGINT, exit_handler);

	isocket = sock_udp_init();
	sock_bind(isocket, argv[2], atoi(argv[3]));
	
	output = fopen(argv[1], "w");
	buffer = malloc(BUFFER_SIZE);
	assert(buffer);

	/* Keep socket open forever */
	while (1) {
		int message_size = 0;
		while ((message_size = sock_receive(isocket, BUFFER_SIZE, buffer))) {
			buffer[message_size] = '\0';
			fwrite(buffer, sizeof(char), message_size, output);
			fflush(output);
		}
	}

	/* We should never actually get down here */
	sock_close(isocket);

	free(buffer);
	fclose(output);

	return 0;
}
