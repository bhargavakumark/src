#include <arpa/inet.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_MSG 1000000
#define LINE_ARRAY_SIZE (MAX_MSG+1)

struct command
{
	float delay;
	float var;
};

int main()
{
  int listenSocket, connectSocket, i,len,ret;
  unsigned short int listenPort;
  socklen_t clientAddressLength;
  struct sockaddr_in clientAddress, serverAddress;
  char line[LINE_ARRAY_SIZE];
  struct command input;


  listenPort = 8000;
  // Create socket for listening for client connection requests.
  listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
	printf("socket create failed");
    exit(1);
  }
  
  // Bind listen socket to listen port.  First set various fields in
  // the serverAddress structure, then call bind().
  // htonl() and htons() convert long integers and short integers
  // (respectively) from host byte order (on x86 this is Least
  // Significant Byte first) to network byte order (Most Significant
  // Byte first).
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(listenPort);
  
  if (bind(listenSocket,
           (struct sockaddr *) &serverAddress,
           sizeof(serverAddress)) < 0) {
    printf("cannot bind socket");
    exit(1);
  }

  // Wait for connections from clients.
  // This is a non-blocking call; i.e., it registers this program with
  // the system as expecting connections on this socket, and then
  // this thread of execution continues on.
  listen(listenSocket, 5);
  
  while (1) {
    printf("Waiting for TCP connection on port %d \n" ,listenPort );

    // Accept a connection with a client that is requesting one.  The
    // accept() call is a blocking call; i.e., this thread of
    // execution stops until a connection comes in.
    // connectSocket is a new socket that the system provides,
    // separate from listenSocket.  We *could* accept more
    // connections on listenSocket, before connectSocket is closed,
    // but this program doesn't do that.
    clientAddressLength = sizeof(clientAddress);
    connectSocket = accept(listenSocket,
                           (struct sockaddr *) &clientAddress,
                           &clientAddressLength);
    if (connectSocket < 0) {
      printf("cannot accept connection ");
      exit(1);
    }
    
    // Show the IP address of the client.
    // inet_ntoa() converts an IP address from binary form to the
    // standard "numbers and dots" notation.
    printf( "  connected to %s" ,inet_ntoa(clientAddress.sin_addr));

    // Show the client's port number.
    // ntohs() converts a short int from network byte order (which is
    // Most Significant Byte first) to host byte order (which on x86,
    // for example, is Least Significant Byte first).

    // Read lines from socket, using recv(), storing them in the line
    // array.  If no messages are currently available, recv() blocks
    // until one arrives.
    // First set line to all zeroes, so we'll know where the end of
    // the string is.
    memset(line, 0x0, LINE_ARRAY_SIZE);
	ret = recv(connectSocket, &input,sizeof(input), 0);
	if(ret < 0)
		printf("cannot receive on socket\n");
	else
	{
		printf("Command delay %f var %f\n",input.delay,input.var);
		system("cnistnet -u\n");
		sprintf(line,"cnistnet -a 192.168.1.10 192.168.1.22 --delay "
			"%f %f 0.8 --drop 2.7 0.8\n",input.delay,input.var);
		system(line);
		sprintf(line,"cnistnet -a 192.168.1.22 192.168.1.10 --delay "
                        "%f %f 0.8 --drop 2.7 1\n",input.delay,input.var);
		system(line);
		system("cnistnet -R\n");
		
	}
  }
}
