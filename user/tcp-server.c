#include <arpa/inet.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/tcp.h>

#define MAX_MSG 1000000
#define LINE_ARRAY_SIZE (MAX_MSG+1)
#define SOL_TCP 6
#define TCP_INFO 11

/*struct tcp_info
{
        unsigned char    tcpi_state;
        unsigned char    tcpi_ca_state;
        unsigned char    tcpi_retransmits;
        unsigned char    tcpi_probes;
        unsigned char    tcpi_backoff;
        unsigned char    tcpi_options;
        unsigned char    tcpi_snd_wscale : 4, tcpi_rcv_wscale : 4;

        unsigned int   tcpi_rto;
        unsigned int   tcpi_ato;
        unsigned int   tcpi_snd_mss;
        unsigned int   tcpi_rcv_mss;

        unsigned int   tcpi_unacked;
        unsigned int   tcpi_sacked;
        unsigned int   tcpi_lost;
        unsigned int   tcpi_retrans;
        unsigned int   tcpi_fackets;

        unsigned int   tcpi_last_data_sent;
        unsigned int   tcpi_last_ack_sent;     
        unsigned int   tcpi_last_data_recv;
        unsigned int   tcpi_last_ack_recv;

        unsigned int   tcpi_pmtu;
        unsigned int   tcpi_rcv_ssthresh;
        unsigned int   tcpi_rtt;
        unsigned int   tcpi_rttvar;
        unsigned int   tcpi_snd_ssthresh;
        unsigned int   tcpi_snd_cwnd;
        unsigned int   tcpi_advmss;
        unsigned int   tcpi_reordering;
};*/


int main()
{
  int listenSocket, connectSocket, i,len,ret;
  unsigned short int listenPort;
  socklen_t clientAddressLength;
  struct sockaddr_in clientAddress, serverAddress;
  char line[LINE_ARRAY_SIZE];
  struct tcp_info info;


  listenPort = 5600;
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
    while (1){
	ret = recv(connectSocket, line, MAX_MSG, 0);
 if(ret < 0)
	break;
    //  printf( "  -- %s\n ",  line );

      // Convert line to upper case.
//      for (i = 0; line[i] != '\0'; i++)
  //      line[i] = toupper(line[i]);

      // Send converted line back to client.
      if (send(connectSocket, line, ret, 0) < 0)
        printf( "Error: cannot send modified data");

	memset((char *)&info, 0x0, sizeof(struct tcp_info));
	len = sizeof(struct tcp_info);
printf("\nCWnd %d SSthresh %d",info.tcpi_snd_cwnd, info.tcpi_snd_ssthresh);

      ret=getsockopt(connectSocket, SOL_TCP,TCP_INFO,(char *)&info,&len); 

     printf("\nRet %d Length %d tcp_info %d",ret, len,sizeof(struct tcp_info));
     printf("\nCWnd %u SSthresh %u\n",info.tcpi_snd_cwnd, info.tcpi_snd_ssthresh);
     printf("\nRTT %u RTTvar %u\n",info.tcpi_rtt,info.tcpi_rttvar);
     printf("\nRetrasnmits %u\n",info.tcpi_retransmits);

  //    memset(line, 0x0, LINE_ARRAY_SIZE);  // set line to all zeroes
    }
  }
}
