/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
/** Implements the Control Server */

#include <vector>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>

#include "Control.h"
#include "Util.h"
#include "JString.h"

void usage(const char *progname) {
  fprintf(stderr, "Usage: %s\n", progname);
} // End usage()

struct sc_node_ctl {
  unsigned short scid;
  struct in_addr ipAddr;
  bool regd;
  int conn_fd;
  sc_node_ctl() { init_fields(); }
  void init_fields() {
    regd = false;
    if(conn_fd != -1) { close(conn_fd); conn_fd = -1; }
  }
};
typedef struct sc_node_ctl sc_node_ctl_t;

int s_fd;
vector<sc_node_ctl_t> sc_list;

void readSCIDMapping(const char *scid_mapping) {
  while(sc_list.size() > 0) { sc_list.pop_back(); }
  vector<sc_node_t> sc1;
  util_readSCIDMapping(scid_mapping, sc1);
  for(unsigned int i = 0; i < sc1.size(); i++) {
    // put the mapping onto the list of sc nodes
    sc_node_ctl_t scn;
    scn.scid = sc1[i].scid;
    scn.ipAddr = sc1[i].ipAddr;
    sc_list.push_back(scn);
  }
} // End readSCIDMapping()

void intr_catcher(int) {
  cerr << "Restarting...";
  for(unsigned int i = 0; i < sc_list.size(); i++) {
    sc_list[i].init_fields();
  }

  Config_t new_cfg; // constructor will re-read all parameters
  if(new_cfg.CONTROL_PORT != Cfg.CONTROL_PORT) {
    cerr << "Can't restart for different CONTROL_PORT, exiting\n";
    exit(0);
  }
  Cfg = new_cfg; // copy new parameters onto old config variable

  readSCIDMapping(Cfg.scid_mapping_file);
  cerr << "done\n";
} // End intr_catcher()

bool main_helper() {
  unsigned int numRegd = 0;
  while(numRegd < sc_list.size()) {

    // using select, so that interrupts are caught (accept call does
    // not return on interrupt)
    fd_set rfds; FD_ZERO(&rfds);
    FD_SET(s_fd, &rfds);
    int sel_res = select(s_fd+1, &rfds, NULL, NULL, NULL);
    if(sel_res < 0) {
      perror("select");
      if(errno == EINTR) { return false; }
      else { exit(82); }
    }
    if(!FD_ISSET(s_fd, &rfds)) {
      cerr << "s_fd not set?\n"; exit(90);
    }

    // get connection from sc node
    struct sockaddr_in cli_addr;
    unsigned int clilen = sizeof(cli_addr);
    int conn_fd = accept(s_fd, (struct sockaddr *)&cli_addr, &clilen);
    if(conn_fd < 0) { perror("accept"); exit(4); }

    // receive regn from sc node
    cPkt_t c2, c1;
    int res = recv(conn_fd, &c2, sizeof(c2), 0);
    if(res != sizeof(c2)) { perror("recv"); exit(5); }
    if(ntohs(c2.cmd) != REGISTER) {
      cerr << "Recv from client scid " << ntohs(c2.scid)
	   << " (" << ip2str(cli_addr.sin_addr) << "): "
	   << ntohs(c2.cmd) << endl;
      exit(6);
    }

    // which sc node did i get it from?
    int ind = -1;
    for(unsigned int i = 0; i < sc_list.size(); i++) {
      if((sc_list[i].ipAddr.s_addr == cli_addr.sin_addr.s_addr) &&
	 (sc_list[i].scid == ntohs(c2.scid))) {
	ind = i;
	break;
      }
    }

    // sanity checks
    if(ind == -1) {
      cerr << "Unknown sc node " << ip2str(cli_addr.sin_addr) << endl;
      exit(8);
    }
    if(sc_list[ind].regd) {
      cerr << "Repeat regn from scid " << sc_list[ind].scid
	   << ' ' << ip2str(sc_list[ind].ipAddr) << endl;
      exit(7);
    }

    // send response
    c1.cmd = htons(REGOK);
    res = send(conn_fd, &c1, sizeof(c1), 0);
    if(res != sizeof(c1)) { perror("send"); exit(8); }

    sc_list[ind].regd = true;
    sc_list[ind].conn_fd = conn_fd;
    numRegd++;

    cerr << "Regn from " << sc_list[ind].scid << ' '
	 << ip2str(sc_list[ind].ipAddr) << endl;

  } // End while() listening for registrations

  cerr << "Sending START to all sc nodes" << endl;
  // issue start command to all sc nodes
  for(unsigned int i = 0; i < sc_list.size(); i++) {
    cPkt_t c1;
    c1.cmd = htons(START);
    int res = send(sc_list[i].conn_fd, &c1, sizeof(c1), 0);
    if(res != sizeof(c1)) { perror("send"); exit(8); }
  }

  for(;;) {
    // read command from STDIN
    cout << "\n"
	 << "Cmd: 'rd'\tAction: read determinstic failure file\n"
	 << "Cmd: 'stop'\tAction: stop all SC nodes\n"
	 << "Cmd: 'quit'\tAction: quit\n"
	 << "Enter cmd: ";
    cout.flush();

    // using select, so that interrupts are caught (accept call does
    // not return on interrupt)
    int stdin_fd = fileno(stdin);
    fd_set rfds; FD_ZERO(&rfds);
    FD_SET(stdin_fd, &rfds);
    int sel_res = select(stdin_fd+1, &rfds, NULL, NULL, NULL);
    if(sel_res < 0) {
      perror("select");
      if(errno == EINTR) { return false; }
      else { exit(82); }
    }
    if(!FD_ISSET(stdin_fd, &rfds)) {
      cerr << "stdin_fd not set?\n"; exit(90);
    }

    char choice_str[1024];
    char *fgets_res = fgets(choice_str, sizeof(choice_str), stdin);
    if(fgets_res == NULL) { break; }
    int len = strlen(fgets_res);
    if(choice_str[len-1] == '\n') { choice_str[len-1] = 0; } // replace '\n'
    JString choice = choice_str;

    if(choice == "quit") { break; }

    cPkt_t c1;
    if(choice == "stop") { c1.cmd = htons(STOP); }
    else if(choice == "rd") { c1.cmd = htons(READ_DET); }
    else { cerr << "Invalid choice '" << choice << "'\n"; continue; }

    cerr << "Sending cmd to all sc nodes" << endl;
    // issue command to all sc nodes
    for(unsigned int i = 0; i < sc_list.size(); i++) {
      int res = send(sc_list[i].conn_fd, &c1, sizeof(c1), 0);
      if(res != sizeof(c1)) { perror("send"); exit(8); }
    }

  } // End for(;;) reading all choices

  for(unsigned int i = 0; i < sc_list.size(); i++) {
    close(sc_list[i].conn_fd);
  }
  close(s_fd);

  return true;
} // End main_helper()

int main(int argc, char *argv[]) {
  if(argc != 1) { usage(argv[0]); exit(1); }

  s_fd = sock_and_listen(Cfg.CONTROL_PORT);
  if(s_fd < 0) { perror("sock_and_listen"); exit(2); }
  cerr << "Listening for registrations on " << Cfg.CONTROL_PORT
       << '\a' << endl;

  // Activate interrupt catcher
  signal(SIGINT, intr_catcher);
  //signal(SIGTERM, intr_catcher);

  readSCIDMapping(Cfg.scid_mapping_file);

  for(;;) {
    if(main_helper()) { break; }
  }

  return 0;
} // End main()
