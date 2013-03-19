
/**
 * socket.h
 * 
 * A suite of functions for handling sockets.
 * 
 * @author Ryan Ashcraft
 */

#ifndef SOCKET_H_
#define SOCKET_H_

/* Descriptor type definition for sockets. */
typedef int sock_desc;

/**
 * Initialize a TCP socket.
 */
sock_desc sock_tcp_init();

/**
 * Initialize a UDP socket.
 */
sock_desc sock_udp_init();

/**
 * Bind a socket to an IP and port. Used for servers.
 * @param descriptor the socket to bind
 * @param ip         the IP address string
 * @param port       the port number
 */
void sock_bind(sock_desc descriptor, const char* ip, unsigned short port);

/**
 * Connect a socket to an IP and port. Used for clients.
 * @param descriptor the socket to connect with
 * @param ip         the IP address string of server
 * @param port       the port number of server
 */
void sock_connect(sock_desc descriptor, const char* ip, unsigned short port);

/**
 * Listen for incoming connections. Used for servers.
 * @param descriptor   the socket to listen on
 * @param max_requests number of simultaneous requests to queue
 */
void sock_listen(sock_desc descriptor, unsigned int max_requests);

/**
 * Accept an incoming connection and return the client socket. Used for servers.
 * @param  descriptor the socket that accepted the connection
 * @return            a new socket for the client
 */
sock_desc sock_accept(sock_desc descriptor);

/**
 * Receive data on a socket. Used for both clients and servers.
 * @param  descriptor  the socket on which to receive data
 * @param  buffer_size the size of the buffer 
 * @param  buffer      the buffer for writing received data
 * @return             the size of the received data
 */
int sock_receive(sock_desc descriptor, unsigned int buffer_size, char* buffer);

/**
 * Send data with a socket. Used for both clients and servers.
 * @param descriptor the socket with which to send data
 * @param size       the size of the message to send
 * @param message    the data to send
 */
void sock_send(sock_desc descriptor, unsigned int size, char* message);

/**
 * Close the socket.
 * @param descriptor the socket to close
 */
void sock_close(sock_desc descriptor);

#endif
