
/* 
 * dsynscan.c - doug's syn scanner
 *
 * 9/12/02 <dcoker at dv8.org>
 * 
 * dsynscan is a half-open TCP port scanner, commonly called a "syn scan".
 * 
 * theory of operation:
 * when run, dscan forks into two processes.  one process uses libnet to
 * send out TCP SYN packets.  the other process uses libpcap and watches
 * for responses from the target host.
 * 
 * as no state is shared between the two processes, it is possible to 
 * fool this program by having the target host send SYNACK in response
 * to SYNs that we never sent.  also, since we have no way of telling
 * when we've received all the results, a timer is used to kill the 
 * program after a user specified period of time.
 * 
 * notes:
 * - must be run as root
 * - some packets may be dropped in transit or not handled by 
 * target when the sleep() and usleep() delays are too low.
 * the delay between packets is not calibrated.
 *
 * requires:
 * - libnet 1.1.x or higher
 * - libpcap (tested on 0.7.1-1) 
 */

#define _BSD_SOURCE 1
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libnet.h>
#include <pcap.h>

#define between(x,l,h) ( ((x) > (l) && (x) < (h)) )

struct opts {
   int  src_port;
   char *target;
   int  low_port;
   int  high_port;
   int  timeout;
} opts;

void sendsyns ()
{
   libnet_t *l;
   char errbuf[LIBNET_ERRBUF_SIZE];
   u_long ip;
   libnet_ptag_t tcp_pkt = LIBNET_PTAG_INITIALIZER;
   libnet_ptag_t ip_pkt;
   u_long dst_ip;
   int  count = 0;
   char *payload = "";		// put your love note here.
   u_short payload_s = strlen (payload);
   int  currentport = opts.low_port;
   int  build_header = 1;

   /* 
    * pcap may be slow to initialize, so let's wait a second. 
    */
   sleep (1);

   /* 
    * initialize libnet for raw IP packets.  libnet will
    *  find an appropriate interface. 
    */
   l = libnet_init (LIBNET_RAW4, NULL, errbuf);
   if (l == NULL) {
      fprintf (stderr, "libnet_init failed: %s", errbuf);
      exit (1);
   }

   /* 
    * get the IP address of the source interface 
    */
   ip = libnet_get_ipaddr4 (l);
   if (-1 == ip) {
      fprintf (stderr, "libnet_get_ipaddr4(l) failed: ");
      fprintf (stderr, "%s\n", libnet_geterror (l));
      exit (1);
   }

   dst_ip = libnet_name2addr4 (l, (u_char *) opts.target, LIBNET_RESOLVE);
   if (-1 == dst_ip) {
      fprintf (stderr, "libnet_name2addr4() failed: ");
      fprintf (stderr, "%s\n", libnet_geterror (l));
      exit (1);
   }

   /* 
    * Build and send a SYN packet, one for each port.  The first time
    *  this loop is executed an IP packet will be constructed. 
    */
   printf ("scanning port %d through %d on %s\n",
	   opts.low_port,
	   opts.high_port, libnet_addr2name4 (dst_ip, LIBNET_DONT_RESOLVE));
   while (currentport <= opts.high_port) {
      /* 
       * Sleep a little... if we send packets too fast, some will get
       * dropped. 
       */
      usleep (50);
      /* 
       * Build the TCP packet with the SYN flag set.  A payload is 
       * optional; for automated scans it is nice to include some text
       * explaining why you are scanning (or ask first!).  Fields with
       * 0 will be calculated by libnet.  
       */
      tcp_pkt = libnet_build_tcp (opts.src_port,	// source port
				  currentport++,	// dest port
				  libnet_get_prand (LIBNET_PRu32),	// seq
				  libnet_get_prand (LIBNET_PRu32),	// ack
				  TH_SYN,	// flags
				  libnet_get_prand (LIBNET_PRu16),	// window size
				  0,	// checksum
				  0,	// urgent ptr
				  LIBNET_TCP_H + payload_s,	// packet size
				  (u_char *) payload,	// contents of packet
				  payload_s,	// size of payload
				  l,	// libnet handle
				  tcp_pkt);	// libnet id
      if (-1 == tcp_pkt) {
	 fprintf (stderr, "libnet_build_tcp() failed: ");
	 fprintf (stderr, "%s\n", libnet_geterror (l));
	 exit (1);
      }

      if (build_header) {
	 build_header = 0;
	 ip_pkt = libnet_autobuild_ipv4 (LIBNET_IPV4_H + LIBNET_TCP_H,
					 IPPROTO_TCP, dst_ip, l);
	 if (-1 == ip_pkt) {
	    fprintf (stderr, "libnet_autobuild_ipv4() failed: ");
	    fprintf (stderr, "%s\n", libnet_geterror (l));
	 }
      }

      /* 
       * Send it! 
       */
      count = libnet_write (l);
      if (-1 == count) {
	 fprintf (stderr, "libnet_write() failed: ");
	 fprintf (stderr, "%s\n", libnet_geterror (l));
      }
   }
   printf ("\n");
}

void
handle_packet (u_char * args, const struct pcap_pkthdr *header,
	       const u_char * packet)
{
   struct ether_header *ethernet = NULL;
   struct ip *ip = NULL;
   struct tcphdr *tcp = NULL;
   int  size_ethernet = sizeof (struct ether_header);
   int  size_ip = sizeof (struct ip);
   int  retcode = 0;

   /* a timer is used to kill the process some number of seconds
    * after the last packet was received.  we do this because we don't
    * know how many packets to expect, and all the packets should arrive
    * within a small timespan
    */
   static struct itimerval timer;
   static struct timeval timeval;

   timeval.tv_sec = 0;
   timeval.tv_usec = 0;
   timer.it_interval = timeval;
   timeval.tv_sec = opts.timeout;
   timer.it_value = timeval;

   /* Set the variables ethernet, ip, and tcp to point to the 
    *  appropriate place in the packet so that we can examine
    *  their fields. 
    */
   ethernet = (struct ether_header *) (packet);
   ip = (struct ip *) (packet + size_ethernet);
   tcp = (struct tcphdr *) (packet + size_ethernet + size_ip);

   /* 
    * We want to make sure we are only seeing IP packets
    *  carrying TCP.  "But, the pcap filter does that already," you say.  
    *  Yes, we did.  But I'm doing it again for good measure.  If a
    *  packet is ever seen here that is not TCP, something is wrong. 
    */
   if (ntohs (ethernet->ether_type) == ETHERTYPE_IP
       && ip->ip_p == IPPROTO_TCP) {
      char *src = strdup (inet_ntoa (ip->ip_src));
      char *dst = strdup (inet_ntoa (ip->ip_dst));
      char *state;
      int  show = 0;

      /* 
       * Test for the flags of the captured packet.
       *  
       *  ack     : half-listening daemon (does this ever happen?)
       *  syn ack : full-listening daemon
       *  ack rst : closed
       *  syn rst : backwards half-listening (does this ever happen?)
       */
      if (tcp->th_flags & TH_SYN && tcp->th_flags && TH_ACK) {
	 show = 1;
	 state = "open";
      } else if (tcp->th_flags & TH_RST) {
	 show = 0;
	 state = "closed";
      } else if (tcp->th_flags & TH_ACK) {
	 show = 1;
	 state = "half open";
      } else {
	 show = 1;
	 state = "something rotten";
      }

      if (show) {
	 printf ("port %d ", ntohs (tcp->th_sport));
	 printf ("%s (%s%s%s%s)", state,
		 (tcp->th_flags & TH_SYN) ? "S" : "s",
		 (tcp->th_flags & TH_ACK) ? "A" : "a",
		 (tcp->th_flags & TH_RST) ? "R" : "r",
		 (tcp->th_flags & TH_PUSH) ? "P" : "p");
	 printf ("\n");
      }

      free (src);
      free (dst);
   } else {
      printf ("Non-ip packet.  Filter broken?\n");
   }

   /* we've received a packet.  if we don't receive a packet in 10 seconds,
    *  have the program quit. 
    */
   retcode = setitimer (ITIMER_REAL, &timer, NULL);
   if (-1 == retcode) {
      perror ("setitimer() failed");
      exit (1);
   }
}

void capture ()
{
   char *dev;
   char errbuf[PCAP_ERRBUF_SIZE];
   pcap_t *capture;
   bpf_u_int32 netp;
   bpf_u_int32 maskp;
   char *filter_string = calloc (100, sizeof (char));
   struct bpf_program filter;
   int  retcode = 0;

   /* create the filter */
   sprintf (filter_string, "dst port %d and tcp", opts.src_port);

   /* find a suitable device */
   dev = pcap_lookupdev (errbuf);
   if (NULL == dev) {
      fprintf (stderr, "pcap_lookupdev() failed: ");
      fprintf (stderr, "%s\n", errbuf);
      exit (1);
   }

   /* lookup parameters of the device returned by pcap_lookupdev.
    *  this information is used when setting the filter. 
    */
   pcap_lookupnet (dev, &netp, &maskp, errbuf);

   /* 
    * pcap_open_live may not return an error code for a warning,
    *  so set errbuf to 0's so that we can test it later.
    */
   memset (errbuf, 0, sizeof (errbuf));

   /* 
    * Open the device for sniffing.  Assume 80 bytes per packet
    *  is enough to analyze results.  Timeout does not apply. 
    */
   capture = pcap_open_live (dev, 80, 0, 10000, errbuf);
   if (NULL == capture) {
      fprintf (stderr, "pcap_open_live() failed: ");
      fprintf (stderr, "%s\n", errbuf);
      exit (1);
   }
   if (strlen (errbuf)) {
      fprintf (stderr, "pcap_open_live() warning: ");
      fprintf (stderr, "%s\n", errbuf);
   }

   /* 
    * Set a filter so that we see only see responses to the source port 
    */
   retcode = pcap_compile (capture, &filter, filter_string, 1, netp);
   if (-1 == retcode) {
      fprintf (stderr, "pcap_compile(%s) failed: ", filter_string);
      fprintf (stderr, "%s\n", pcap_geterr (capture));
   }

   /* 
    * Attach the filter to the capture device 
    */
   retcode = pcap_setfilter (capture, &filter);
   if (-1 == retcode) {
      fprintf (stderr, "pcap_setfilter(%s) failed: ", filter_string);
      fprintf (stderr, "%s\n", pcap_geterr (capture));
   }

   /* 
    * pcap_loop will call our handle_packet() function repeatedly
    *  until 2048 packets are captured. 
    */
   retcode = pcap_loop (capture, 2048, &handle_packet, (u_char *) "meow");

   printf ("pcap_loop returned %d\n", retcode);

   pcap_close (capture);
}

int main (int argc, char **argv)
{
   int  retcode = 0;
   char opt;

   /* 
    * Unbuffer stdout 
    */
   setbuf (stdout, NULL);

   /* 
    * default configuration options 
    */
   opts.src_port = 31337;
   opts.low_port = 1;
   opts.high_port = 1024;
   opts.target = NULL;
   opts.timeout = 5;

   if (argc < 2) {
      fprintf (stderr, "usage: %s\n", argv[0]);
      fprintf (stderr, "\t-d target\n");
      fprintf (stderr, "\t-s source_port (default %d)\n", opts.src_port);
      fprintf (stderr, "\t-l low_port (default %d)\n", opts.low_port);
      fprintf (stderr, "\t-h high_port (default %d)\n", opts.high_port);
      fprintf (stderr, "\t-t timeout_in_seconds (default %d)\n",
	       opts.timeout);
      return 1;
   }

   /* 
    * parse command line options 
    */
   opterr = 0;
   while ((opt = getopt (argc, argv, "s:d:l:h:t:")) != -1) {
      switch (opt) {
      case 't':
	 opts.timeout = atoi (optarg);
	 break;
      case 's':
	 opts.src_port = atoi (optarg);
	 if (!(between (opts.src_port, 0, 65536))) {
	    fprintf (stderr, "source port must be between 1 and 65536\n");
	    return (1);
	 }
	 break;
      case 'd':
	 opts.target = strdup (optarg);
	 break;
      case 'l':
	 opts.low_port = atoi (optarg);
	 if (!(between (opts.low_port, 0, 65536))) {
	    fprintf (stderr, "low port must be between 1 and 65536\n");
	    return (1);
	 }
	 break;
      case 'h':
	 opts.high_port = atoi (optarg);
	 if (!(between (opts.high_port, 0, 65536))) {
	    fprintf (stderr, "high port must be between 1 and 65536\n");
	    return (1);
	 }
	 break;
      case '?':
	 fprintf (stderr, "invalid command line\n");
	 return 1;
      }
   }

   /* 
    * make sure the arguments are sane 
    */
   if (opts.low_port > opts.high_port) {
      fprintf (stderr, "low port must be <= high port\n");
      return 1;
   }
   if (1 > opts.low_port || 1 > opts.high_port) {
      fprintf (stderr, "port numbers must be >= 1");
      return 1;
   }
   if (!opts.target) {
      fprintf (stderr, "target must be specified.\n");
      return (1);
   }

   /* 
    * Fork to capture() and sendsyns() 
    */
   if (0 == (retcode = fork ())) {
      capture ();
   } else if (retcode == -1) {
      fprintf (stderr, " unable to fork ");
      return 1;
   } else {
      sendsyns ();
   }
   return 0;
}
