
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket.h"

sock_desc sock_tcp_init() {
    sock_desc descriptor;

	if ((descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		fprintf(stderr, "%s\n", "socket could not be initialized");
        exit(-1);
    }

    return descriptor;
}

sock_desc sock_udp_init() {
    sock_desc descriptor;

    if ((descriptor = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        fprintf(stderr, "%s\n", "socket could not be initialized");
        exit(-1);
    }

    return descriptor;
}

void sock_bind(sock_desc descriptor, const char* ip, unsigned short port) {
	struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    if (bind(descriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
    	fprintf(stderr, "%s %d\n", "socket could not be binded to port", port);
        exit(-1);
    }

    fprintf(stderr, "%s %d\n", "socket bound to port", port);
}

void sock_connect(sock_desc descriptor, const char* ip, unsigned short port) {
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    if (connect(descriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
        fprintf(stderr, "%s %d\n", "socket could not be connected to port", port);
        exit(-1);
    }

    fprintf(stderr, "%s %d\n", "socket connected on port", port);
}

void sock_listen(sock_desc descriptor, unsigned int max_requests) {
    if (listen(descriptor, max_requests) < 0) {
        fprintf(stderr, "%s\n", "socket listen failed"); 
        exit(-1);
    }

    fprintf(stderr, "%s\n", "socket listening");
}

sock_desc sock_accept(sock_desc descriptor) {
	sock_desc client_descriptor;
	struct sockaddr_in client_address;
	unsigned int client_address_size = sizeof(client_address);

    /* Wait to accept */
    if ((client_descriptor = accept(descriptor, (struct sockaddr *) &client_address, &client_address_size)) < 0) {
        fprintf(stderr, "%s\n", "socket accept failed");
        exit(-1);
   }

   return client_descriptor;
}

int sock_receive(sock_desc descriptor, unsigned int buffer_size, char* buffer) {
    int message_size;

    if ((message_size = recv(descriptor, buffer, buffer_size, 0)) < 0) {
    	fprintf(stderr, "%s\n", "socket receive failed");
    	exit(-1);
    }

    return message_size;
}

void sock_send(sock_desc descriptor, unsigned int size, char* message) {
	if ((int)send(descriptor, message, size, 0) != size) {
		fprintf(stderr, "%s\n", "socket send failed");
        exit(-1);
	}
}

void sock_close(sock_desc descriptor) {
    close(descriptor);
}
