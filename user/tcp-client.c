#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_LINE 100
#define LINE_ARRAY_SIZE (MAX_LINE+1)


int main()
{
  int socketDescriptor;
  unsigned short int serverPort;
  struct sockaddr_in serverAddress;
  struct hostent *hostInfo;
  char buf[LINE_ARRAY_SIZE], c;
  int windowsize;
  int len,ret;

  serverPort = 5600;
  strcpy(buf,"127.0.0.1");

  // gethostbyname() takes a host name or ip address in "numbers and
  // dots" notation, and returns a pointer to a hostent structure,
  // which we'll need later.  It's not important for us what this
  // structure is actually composed of.
  hostInfo = gethostbyname(buf);
  if (hostInfo == NULL) {
    printf( "problem interpreting host: %s \n", buf );
    exit(1);
  }

  // "SOCK_STREAM" means it will be a reliable connection (i.e., TCP;
  // for UDP use SOCK_DGRAM), and I'm not sure what the 0 for the last
  // parameter means, but it seems to work.
  socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (socketDescriptor < 0) {
    printf( "cannot create socket\n");
    exit(1);
  }

  // Connect to server.  First we have to set some fields in the
  // serverAddress structure.  The system will assign me an arbitrary
  // local port that is not in use.
  serverAddress.sin_family = hostInfo->h_addrtype;
  memcpy((char *) &serverAddress.sin_addr.s_addr,
         hostInfo->h_addr_list[0], hostInfo->h_length);
  serverAddress.sin_port = htons(serverPort);
	
  printf("\n Source addr %u\n",serverAddress.sin_addr.s_addr);

  len = sizeof(windowsize);
  windowsize = 524287;

  ret=setsockopt(socketDescriptor, SOL_SOCKET,SO_RCVBUF,(char *)&windowsize,len);
  printf("Socket recv buffer size %d\n",windowsize);
  ret=setsockopt(socketDescriptor, SOL_SOCKET,SO_SNDBUF,(char *)&windowsize,len);
  printf("Socket recv buffer size %d\n",windowsize);	

  ret=getsockopt(socketDescriptor, SOL_SOCKET,SO_RCVBUF,(char *)&windowsize,&len);
  printf("Socket recv buffer size %d\n",windowsize);
  ret=getsockopt(socketDescriptor, SOL_SOCKET,SO_SNDBUF,(char *)&windowsize,&len);
  printf("Socket recv buffer size %d\n",windowsize);			
  if (connect(socketDescriptor,
              (struct sockaddr *) &serverAddress,
              sizeof(serverAddress)) < 0) {
    printf( "cannot connect\n");
    exit(1);
  }


  // Prompt the user for input, then read in the input, up to MAX_LINE
  // charactars, and then dispose of the rest of the line, including
  // the newline character.

  strcpy(buf,"hello world.");

  // Stop when the user inputs a line with just a dot.
  while (1) {
	strcpy(buf,"hellow world.");
    // Send the line to the server.
    if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0) {
      printf( "cannot send data ");
      close(socketDescriptor);
      exit(1);
    }

    // Zero out the buffer.
    memset(buf, 0x0, LINE_ARRAY_SIZE);

    // Read the modified line back from the server.
    if (recv(socketDescriptor, buf, MAX_LINE, 0) < 0) {
      printf( "didn't get response from server?");
      close(socketDescriptor);
      exit(1);
    }

    printf( "Modified: %s" ,buf);

    // Prompt the user for input, then read in the input, up to MAX_LINE
    // charactars, and then dispose of the rest of the line, including
    // the newline character.  As above.
  }

  close(socketDescriptor);
  return 0;
}
