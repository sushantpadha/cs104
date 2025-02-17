/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
#ifndef CONFIG_H
#define CONFIG_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

#include "JString.h"

struct cfg_kv {
  JString key;
  JString value;
};
typedef struct cfg_kv cfg_kv_t;

class Config {
public:
  Config();

  /** This machine's IP address */
  struct in_addr myIP;

  /** Tunneling configuration */
  char TUN_HOST[512];
  struct in_addr tun_ip;
  int TUN_PORT;

  /** Setting for global debug output */
  bool gdbg;

  /** Debug configuration */
  bool cfg_to_file;
  bool cfg_use_stream;
  long logLimit;

  /** CM configuration */
  bool cfg_read_paths;
  bool cfg_opt_logline;
  bool only_odd_path_recovery;
  bool multi_sc;
  int port_mult;

  /** Port configuration for CM */
  int SC_PORT_BASE;
  int HB_PORT;
  int LAT_MEAS_PORT;
  int LINK_STATE_PORT;
  int LINK_STATE_SEND_ACK_PORT;
  int LINK_STATE_FWD_ACK_PORT;
  int SERV_INFO_PORT;
  int SERV_INFO_ACK_PORT;
  int APP_IF_PORT;
  int SERV_COMP_PORT;
  int SERV_COMP_ACK_PORT;
  int SERV_COMP_SERVER_PORT;
  int SERV_COMP_CLIENT_PORT;

  /** Control server configuration */
  int CONTROL_PORT;
  char controlServer[512];

  /** Time server configuration */
  int TIME_SYNC_PORT;
  char timeServer[512];

  /** More CM configuration */
  char read_paths_file[1024];
  char scid_mapping_file[1024];
  char graph_file[1024];
  char service_locn_file[1024];

  /** Runtime options for the CM */
  bool only_e2e; /* Should use only end-to-end recovery? */
  bool bunched; /* Should I do bunched recovery? */
  bool rate_control; /* Should I do rate control in recovery? */
  bool joint_listen; /* Should I do a joint listen? */
  bool use_queue; /* Should I use a queue of request numbers, or a
		     vector indexed on the reqNo? */
  int HB_TIMEOUT; /** The heart-beat timeout, in ms */

  /** fwdRaw configuration */
  char det_fail_file[1024];
  int normalizedMaxCost;

  /** Emulator upcall configuration */
  bool use_emul_upcalls;

  /** Client configuration */
  bool cfg_read_path_params;
  int numPaths;
  int CLIENT_PORT;

  /** Services configuration */
  int SERVICE_PORT;
  int noop_sc_listen_port;
  int tts_sc_listen_port;
  int email_sc_listen_port;

  /** Various other configuration options and (otherwise) command-line
      options */
  vector<cfg_kv_t> kv;

  /** Get the value corresponding to the given key (NULL string on
      error) */
  JString get_cfg_value(const JString &key);
  /** Get the double value corresponding to the given key, the default
      value is given in 'deflt'. */
  double get_cfg_double(const JString &key, double deflt);
  /** Get the long value corresponding to the given key, the default
      value is given in 'deflt'. */
  long get_cfg_long(const JString &key, long deflt);
  /** Get the boolean value corresponding to the given key, the
      default value is given in 'deflt'. */
  bool get_cfg_bool(const JString &key, bool deflt);
  /** Get the string value corresponding to the given key, the default
      value is given in 'deflt' */
  char *get_cfg_str(const JString &key, char *val, const char *deflt);
  /** Prepend home directory to given string */
  void prepend_home(char *s);

protected:
  /** Read the key-value config file.  Format is:
      key1=value1
      key2=value2
      ...
      isspace() characters in the beginning of the line are ignored.
      Comment lines begin with '#' (but for isspace() characters), or
      are empty (but for isspace() characters).

      Config file is one of the following, in that order:
      - That given by the environment variable $SC_CFG
      - $HOME/.sc.cfg
  */
  void read_kv_file();

}; // End class Config
typedef class Config Config_t;

extern Config_t Cfg;

#endif
