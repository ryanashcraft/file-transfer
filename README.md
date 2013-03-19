# CS 3251 Problem Set 2

`make all` generates four binaries in the `bin` directory: tcpserver, tcpclient, udpserver, and udpclient. Each have the same required runtime arguments: the input/output filename, the IP address (to bind or connect to), and the port number (again to bind or connect to).

## Examples

### TCP Server
	./bin/tcpserver received_photo.png 127.0.0.1 8000

### TCP Client
	./bin/tcpclient photo.png 127.0.0.1 8000

### UDP Server
	./bin/udpserver received_photo.png 127.0.0.1 8000

### UDP Client
	./bin/udpclient photo.png 127.0.0.1 8000