#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>

#define MAX_LINE 100
#define LINE_ARRAY_SIZE (MAX_LINE+1)


struct command
{
        float delay;
        float var;
};

int main(int argc, char *argv[])
{
  int socketDescriptor;
  unsigned short int serverPort;
  struct sockaddr_in serverAddress;
  struct sockaddr_in temp;
  struct hostent *hostInfo;
  char buf[LINE_ARRAY_SIZE], c;
  int windowsize;
  int len,ret;
  struct command input;

  serverPort = 8000;
  strcpy(buf,"127.0.0.1");

  if(argc!=3)
  {
	printf("%s <delay> <SD in delay>\n",argv[0]);
	exit(-1);
  }

  input.delay = atof(argv[1]);
  input.var = atof(argv[2]);
  printf("delay %f var %f\n",input.delay,input.var);

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

  if (connect(socketDescriptor,
              (struct sockaddr *) &serverAddress,
              sizeof(serverAddress)) < 0) {
    printf( "cannot connect\n");
    exit(1);
  }

  len = sizeof(struct sockaddr);
  getsockname(socketDescriptor, &temp, &len);
  printf("sock port %d %d \n",temp.sin_port,htons(temp.sin_port));
  getpeername(socketDescriptor,&temp,&len);
  printf("peer port %d %d \n",temp.sin_port,htons(temp.sin_port));

    if (send(socketDescriptor, &input,sizeof(input), 0) < 0)
	      printf( "cannot send data ");

  close(socketDescriptor);
  return 0;
}
