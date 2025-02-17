/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "Util.h"
#include "Config.h"

Config_t Cfg;

Config::Config() {
  //---- Get this machine's IP address ----//
  int ip_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
  struct ifconf ifs;
  struct ifreq ifr[5];
  ifs.ifc_len = 5*sizeof(struct ifreq);
  ifs.ifc_req = ifr;
  int res = ioctl(ip_fd, SIOCGIFCONF, &ifs);
  if(res != 0) { perror("Error in ioctl call"); exit(3); }

  for(unsigned int i = 0; i < ifs.ifc_len/sizeof(struct ifreq); i++) {
    if(ioctl(ip_fd, SIOCGIFFLAGS, &ifr[i]) < 0) {
      continue;
    }

    /*
    fprintf(stderr, "%s %d %d\n", ifr[i].ifr_name,
	    (int)(ifr[i].ifr_flags & IFF_LOOPBACK),
	    (int)(ifr[i].ifr_flags & IFF_UP));
    */

    if((!(ifr[i].ifr_flags & IFF_LOOPBACK)) &&
       (ifr[i].ifr_flags & IFF_UP)) {
      if(ioctl(ip_fd, SIOCGIFADDR, &ifr[i]) < 0) {
	//cerr << "Ignoring SIOCGIFADDR " << ip2str(myIP) << endl;
	continue;
      }
      myIP = (*((struct sockaddr_in *)&ifr[i].ifr_addr)).sin_addr;

      if(ioctl(ip_fd, SIOCGIFINDEX, &ifr[i]) < 0) {
	//cerr << "Ignoring SIOCGIFINDEX " << ip2str(myIP) << endl;
	continue;
      }
      struct in_addr network;
      // inet_netof takes in n/w byte order, returns host byte order
      network.s_addr = htonl(inet_netof(myIP));
      if(network.s_addr == inet_network("10.0.0.0")) {
	//cerr << "Ignoring 10.0.0.0 address " << ip2str(myIP) << endl;
	continue;
      }
      //cerr << "Accepting " << ip2str(myIP) << " " << ip2str(network) << endl;
      break;
    }
  }
  close(ip_fd);

  /*
  const char *hostname = getenv("HOSTNAME");
  if(hostname == NULL) { fprintf(stderr, "can't get HOSTNAME\n"); exit(4); }
  //myIP = nslookup(hostname);
  if(strcasecmp(hostname, "laptop.bdhome") == 0) {
    myIP = nslookup("127.0.0.1");
  }
  if(strcasecmp(hostname, "localhost.localdomain") == 0) {
    myIP = nslookup("127.0.0.1");
  }
  */
  bool all_local = get_cfg_bool("all_local", false);
  if(all_local) { myIP = nslookup("127.0.0.1"); }

  //---- Read the configuration file ----//
  read_kv_file();

  //---- Init with default values ----//
  get_cfg_str("TUN_HOST", TUN_HOST, "169.229.48.162");
  TUN_PORT = get_cfg_long("TUN_PORT", 8866);
  tun_ip = nslookup(TUN_HOST);

  gdbg = get_cfg_bool("gdbg", false);

  cfg_to_file = get_cfg_bool("cfg_to_file", false);
  cfg_use_stream = get_cfg_bool("cfg_use_stream", false);
  logLimit = get_cfg_long("logLimit", 0);

  cfg_read_paths = get_cfg_bool("cfg_read_paths", true);
  cfg_opt_logline = get_cfg_bool("cfg_opt_logline", false);
  only_odd_path_recovery = get_cfg_bool("only_odd_path_recovery", false);
  multi_sc = get_cfg_bool("multi_sc", false);
  port_mult = get_cfg_long("port_mult", 15);

  SC_PORT_BASE = get_cfg_long("SC_PORT_BASE", 10000);
  HB_PORT = get_cfg_long("HB_PORT", 10000);
  LAT_MEAS_PORT = get_cfg_long("LAT_MEAS_PORT", 10001);
  LINK_STATE_PORT = get_cfg_long("LINK_STATE_PORT", 10002);
  LINK_STATE_SEND_ACK_PORT = get_cfg_long("LINK_STATE_SEND_ACK_PORT", 10003);
  LINK_STATE_FWD_ACK_PORT = get_cfg_long("LINK_STATE_FWD_ACK_PORT", 10004);
  SERV_INFO_PORT = get_cfg_long("SERV_INFO_PORT", 10005);
  SERV_INFO_ACK_PORT = get_cfg_long("SERV_INFO_ACK_PORT", 10006);
  APP_IF_PORT = get_cfg_long("APP_IF_PORT", 10007);
  SERV_COMP_PORT = get_cfg_long("SERV_COMP_PORT", 10008);
  SERV_COMP_ACK_PORT = get_cfg_long("SERV_COMP_ACK_PORT", 10009);
  SERV_COMP_SERVER_PORT = get_cfg_long("SERV_COMP_SERVER_PORT", 10010);
  SERV_COMP_CLIENT_PORT = get_cfg_long("SERV_COMP_CLIENT_PORT", 10011);

  get_cfg_str("controlServer", controlServer, "169.229.48.90");
  CONTROL_PORT = get_cfg_long("CONTROL_PORT", 3232);

  get_cfg_str("timeServer", timeServer, "169.229.48.90");
  TIME_SYNC_PORT = get_cfg_long("TIME_SYNC_PORT", 7744);

  get_cfg_str("read_paths_file", read_paths_file, "/dev/null");
  prepend_home(read_paths_file);
  get_cfg_str("scid_mapping_file", scid_mapping_file, "/dev/null");
  prepend_home(scid_mapping_file);
  get_cfg_str("graph_file", graph_file, "/dev/null");
  prepend_home(graph_file);
  get_cfg_str("service_locn_file", service_locn_file, "/dev/null");
  prepend_home(service_locn_file);

  only_e2e = get_cfg_bool("only_e2e", true);
  bunched = get_cfg_bool("bunched", true);
  rate_control = get_cfg_bool("rate_control", true);
  joint_listen = get_cfg_bool("joint_listen", true);
  use_queue = get_cfg_bool("use_queue", false);
  HB_TIMEOUT = get_cfg_long("HB_TIMEOUT", 1800); // 1.8sec

  get_cfg_str("det_fail_file", det_fail_file, "/dev/null");
  prepend_home(det_fail_file);
  /* 100ms is the normalized max cost (one way) of overlay links */
  normalizedMaxCost = get_cfg_long("normalizedMaxCost", 100);

  use_emul_upcalls = get_cfg_bool("use_emul_upcalls", false);

  numPaths = get_cfg_long("numPaths", 0);
  // if client is not reading path parameters, SC should not be reading paths
  cfg_read_path_params = get_cfg_bool("cfg_read_path_params", false);
  cfg_read_paths = (cfg_read_paths && cfg_read_path_params);
  CLIENT_PORT = get_cfg_long("CLIENT_PORT", 19999);

  SERVICE_PORT = get_cfg_long("SERVICE_PORT", 23999);
  noop_sc_listen_port = get_cfg_long("noop_sc_listen_port", 23999);
  tts_sc_listen_port = get_cfg_long("tts_sc_listen_port", 24999);
  email_sc_listen_port = get_cfg_long("email_sc_listen_port", 25999);

} // End Config::Config()

void Config::prepend_home(char *s) {
  if(s[0] == '/') { return; } // absolute path given already
  char temp[1024];
  strcpy(temp, s);
  sprintf(s, "%s/%s", getenv("HOME"), temp);
} // End Config::prepend_home()

void Config::read_kv_file() {

  char kv_file[1024];
  char *env_ptr = getenv("SC_CFG");
  if(env_ptr == NULL) { sprintf(kv_file, "%s/.sc.cfg", getenv("HOME")); }
  else { strcpy(kv_file, env_ptr); }

  ifstream fin(kv_file);
  if(fin.fail()) {
    cerr << "Cannot open config file " << kv_file << endl;
    exit(98);
  }

  // empty existing vector of key-value pairs
  while(kv.size() > 0) { kv.pop_back(); }

  //cerr << "Reading config file \"" << kv_file << "\"...";
  for(;;) {
    // read each line
    if(fin.eof()) break;
    char line[1024];
    fin.getline(line, sizeof(line)); // read a line
    if(fin.eof()) break;
    if(fin.fail()) {
      cerr << "Error reading " << kv_file << endl;
      exit(99);
    }

    // ignore white spaces in the beginning
    char *line_start = line;
    while(*line_start && isspace(*line_start)) { line_start++; }

    // ignore empty lines
    if(strlen(line_start) == 0) { continue; }
    // ignore commented lines
    if(*line_start == '#') { continue; }

    // look for the '=' sign
    char *eq_pos = index(line_start, '=');
    if(eq_pos == NULL) { cerr << "'=' not found" << endl; exit(100); }
    *eq_pos = 0;

    // insert the key-value pair into the vector
    cfg_kv_t pair;
    pair.key = line_start;
    pair.value = (eq_pos+1);
    kv.push_back(pair);

  } // End for(;;) reading all lines in config file
  //cerr << " done.\n";

  fin.close();

} // End Config::read_kv_file()

JString Config::get_cfg_value(const JString &key) {
  for(unsigned int i = 0; i < kv.size(); i++) {
    if(kv[i].key == key) return kv[i].value;
  }
  return "";
} // End Config::get_value()

double Config::get_cfg_double(const JString &key, double deflt) {
  const JString &val_str = get_cfg_value(key);
  if(val_str == "") { return deflt; }
  else { return atof(val_str.cstr()); }
} // End Config::get_cfg_double()

long Config::get_cfg_long(const JString &key, long deflt) {
  const JString &val_str = get_cfg_value(key);
  if(val_str == "") { return deflt; }
  else { return atol(val_str.cstr()); }
} // End Config::get_cfg_long()

bool Config::get_cfg_bool(const JString &key, bool deflt) {
  const JString &val_str = get_cfg_value(key);
  if(val_str == "") { return deflt; }
  else { return (bool)(atoi(val_str.cstr())); }
} // End Config::get_cfg_bool()

char *Config::get_cfg_str(const JString &key, char *val, const char *deflt) {
  const JString &val_str = get_cfg_value(key);
  if(val_str == "") { strcpy(val, deflt); }
  else { strcpy(val, val_str.cstr()); }
  return val;
} // End Config::get_cfg_str()
