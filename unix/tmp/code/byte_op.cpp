/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
/** Command-line forms of htonl, ntohl, htons, ntohs */

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(const char *progname) {
  fprintf(stderr, "Usage: %s <htonl/ntohl/htons/ntohs> <number>\n", progname);
} // End usage()

int main(int argc, char *argv[]) {

  if(argc != 3) { usage(argv[0]); exit(1); }

  const char *conv_type = argv[1];
  const char *num_ptr = argv[2];

  long num = 0, conv_num = 0;
  if(strcmp(conv_type, "htonl") == 0) {
    num = atol(num_ptr); conv_num = htonl(num);
  } else if(strcmp(conv_type, "ntohl") == 0) {
    num = atol(num_ptr); conv_num = ntohl(num);
  } else if(strcmp(conv_type, "htons") == 0) {
    num = atol(num_ptr); conv_num = htons(num);
  } else if(strcmp(conv_type, "ntohs") == 0) {
    num = atol(num_ptr); conv_num = ntohs(num);
  } else {
    usage(argv[0]); exit(2);
  }

  fprintf(stderr, "%s(%ld) = %ld\n", conv_type, num, conv_num);

} // End main()
