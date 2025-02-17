/** Program to test when one is allowed to send to a TCP socket, and
    whether it has any relation to actual packet send on the
    network */

#include <stdio.h>
#include <unistd.h>

#include "Util.h"

#define TCP_PORT 1200
#define SERVER_MODE 0
#define CLIENT_MODE 1

void usage(const char *progname) {
  fprintf(stderr, "Usage: %s <-s/-c> [remote-port] [remote-host]\n", progname);
} // End usage()

void server(int port) {

  int s = sock_and_listen(port);
  if(s == -1) { perror("sock_and_listen"); exit(100); }

  fprintf(stderr, "Listening on port %d\n\a", port);

  struct sockaddr_in cl_addr;
  int cl_addr_size = sizeof(cl_addr);
  int cs = accept(s, (struct sockaddr*)&cl_addr, (socklen_t*)&cl_addr_size);
  char cl_str[1024];
  ipPrint(cl_str, cl_addr.sin_addr);
  fprintf(stderr, "Connection from %s:%d\n\a", cl_str, ntohs(cl_addr.sin_port));

  for(;;) {

    char buf[4096];
    int num = recv(cs, buf, sizeof(buf), 0);
    if(num <= 0) { break; }

    fprintf(stderr, "[%d]", num);

  } // End for(;;)

  fprintf(stderr, "\n\a");

  close(cs);
  close(s);

} // End server()

void client(const char *host, int port) {

  struct in_addr server_addr = nslookup(host);
  fprintf(stderr, "Trying to connect to %s:%d\n",
	  ip2str(server_addr).cstr(), port);
  int s = sock_and_connect(host, port);
  if(s == -1) { perror("client"); exit(200); }

  for(int i = 0; i < 1000; i++) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(s, &fds);
    int max_fd = s;

    int num_ready = select(max_fd+1, NULL, &fds, NULL, 0);
    if(num_ready == 0) {
      fprintf(stderr, "Can't send now\n\a");
      break;
    }

    char buf[1448];
    int num_sent = send(s, buf, sizeof(buf), MSG_DONTWAIT);
    fprintf(stderr, "[%d]", num_sent);
  } // End for(;;)

  fprintf(stderr, "\n\a");
  close(s);

} // End client()

int main(int argc, char *argv[]) {

  if(argc < 2) { usage(argv[0]); exit(1); }

  int mode = -1;
  if(strcmp(argv[1], "-s") == 0) { mode = SERVER_MODE; }
  else if(strcmp(argv[1], "-c") == 0) { mode = CLIENT_MODE; }
  else { usage(argv[0]); exit(2); }

  char host[1024];
  int port = -1;

  strcpy(host, "localhost");
  port = 1200;
  if(argc == 3) { port = atoi(argv[2]); }
  if(argc == 4) { strcpy(host, argv[3]); }
  if(argc > 4) { usage(argv[0]); exit(3); }

  if(mode == SERVER_MODE) { server(port); }
  else { client(host, port); }

  return 0;
} // End main()
