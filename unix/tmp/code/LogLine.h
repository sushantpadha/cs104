/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
#ifndef LOGLINE_H
#define LOGLINE_H

#include <vector>
#include <netinet/in.h>

#include "JString.h"

enum desc_type {
  invalid_desc,

  ack_for_sending_e2e_patch_towards_dest_desc,
  add_path_desc,
  alt_done_desc,
  alt_path_in_progress_desc,
  am_in_repaired_sub_path_desc,
  am_last_in_sub_path_desc,
  am_not_in_posn_desc,
  am_not_in_sub_posn_desc,
  am_part_of_orig_path_desc,
  am_upstream_of_sub_path_desc,
  path_id_not_valid_desc,

  bug_ack_from_non_peer_desc,
  bunched_pkt_too_big_desc,

  ca_timeout_desc,
  cp_done_desc,
  cp_fail_desc,
  cp_req_desc,
  cp_send_upstream_desc,
  create_alt_path_desc,
  creating_entry_for_path_desc,
  csi_req_desc,
  cs_req_desc,

  dead_links_desc,
  del_non_existent_path_desc,
  detect_time_desc,
  drop_start_desc,
  drop_end_desc,
  dup_hb_desc,
  dup_req_desc,
  dup_req_for_curr_version_desc,

  e2e_patch_desc,
  e2e_req_desc,
  edge_repeat_try_e2e_desc,
  err_alt_created_but_not_used_desc,
  err_incomplete_alt_path_in_use_desc,

  graph_link_dead_desc,
  graph_link_live_desc,
  graph_state_stale_desc,

  hb_ack_desc,
  hb_req_desc,

  incoming_dead_desc,
  incoming_restored_desc,
  incomplete_path_desc,
  invalid_num_in_bunch_desc,
  invalid_posn_desc,
  invalid_sub_posn_desc,

  leave_for_downstream_desc,
  local_exists_cant_handle_desc,
  local_fail_try_e2e_desc,
  lost_ack_desc,
  lost_ack_for_curr_version_desc,
  lost_ack_for_old_version_desc,
  lost_ack_for_old_version_alt_in_construction_desc,
  lost_ack_for_past_version_desc,
  ls_update_from_unknown_peer_desc,

  new_sub_path_weird_desc,
  new_sub_path_weird_try_e2e_desc,
  node_repeat_try_e2e_desc,
  non_existent_edge_del_path_desc,
  non_existent_edge_ignoring_desc,
  non_existent_path_desc,
  no_phys_path_desc,
  no_recognized_sub_path_desc,
  no_replacement_try_e2e_desc,
  nothing_to_patch_desc,
  num_in_bunch_desc,

  outgoing_dead_desc,

  parse_err_desc,
  pass_ack_desc,
  pass_req_upstream_desc,
  path_complete_desc,
  path_ends_here_desc,
  path_exists_here_desc,
  path_not_found_desc,
  paths_unequal_desc,
  pc_send_upstream_desc,
  peer_cost_change_desc,
  peer_link_dead_desc,
  peer_link_live_desc,
  phys_path_found_desc,
  pkt_drop_desc,

  rcv_fail_desc,
  removing_old_path_hb_entry_desc,
  repair_done_desc,
  repair_exists_cant_handle_desc,
  repair_exists_try_e2e_desc,
  repair_not_in_db_desc,
  replacement_found_desc,
  req_desc,

  sc_stopped_not_starting_recovery_desc,
  send_or_fwd_hb_desc,
  send_or_fwd_hb_to_unknown_peer_desc,
  send_req_upstream_desc,
  stale_to_patch_desc,
  start_desc,
  start_dead_desc,
  start_live_desc,
  sub_path_is_weird_desc,

  timing_out_path_desc,
  timing_out_sub_path_desc,
  total_recd_desc,
  total_sent_desc,
  try_local_desc,
  try_patch_desc,

  unequal_paths_desc,
  unknown_peer_desc,
  unknown_repair_path_desc,

  version_mismatch_desc,

  path_cost_desc,
  repair_path_cost_desc,
  alt_path_cost_desc,

  emul_stats_desc,
  cpu_load_desc,
  edge_load_desc,

  path_killed_before_alt_desc,
  path_killed_before_complete_desc,
  path_killed_before_repair_desc,
  path_killed_not_starting_recovery_desc,
  path_killed_before_serv_ack_desc,

  err_decr_load_desc,

  only_odd_path_recovery_desc,
  log_limit_desc,

  initial_active_desc,
  set_active_desc,
  set_inactive_desc,

  app_listen_start_desc,
  app_dup_req_desc,
  app_no_free_cl_desc,
  app_create_serv_desc,
  app_change_serv_desc,
  app_kill_serv_desc,

  app_send_upstream_info_desc,
  app_recv_upstream_info_desc,
  app_ctl_ack_desc,
  app_recv_hb_desc,
  app_send_hb_desc,
  app_path_timeout_desc,
  app_path_kill_desc,

  app_unknown_pkt_desc,

  app_tts_req_desc,
  app_data_ack_desc,
  app_unknown_pkt_type_desc,
  app_no_state_str_desc,
  app_no_col_desc,
  app_ok_desc,
  app_ok_reorder_desc,
  app_sc_reply_desc,

  app_all_sentences_done_desc,
  app_tts_resp_desc,
  app_spawn_thread_desc,
  app_thread_done_desc,
  app_data_ack_timeout_desc,
  app_send_soft_state_desc,
};

struct desc_type_name {
  enum desc_type type;
  char name[64];
};

enum fn_type {
  INVALID_FN,

  DEBUG_FN,

  SENDTHREAD_FN, LISTENTHREAD_FN,

  PKTDROP_FN,

  TIMEOUTPATH_FN,
  CHECKFAILURES_FN, PATCHPATHSFROMPEER_FN, PATCHPATHSFROMPEERRC_FN,
  CHECKPATCHQ_FN, PATCHPATH_FN, PATCHEDGELOCAL_FN,
  PATCHEDGEE2EHELPER_FN, PEERPATCHPATHLOCAL_FN, PEERPATCHBUNCH_FN,
  PEERPATCHPATHE2E_FN, SENDBUNCH_FN,

  CREATEPATH_FN, CREATESERVINST_FN, PEERCREATESERVINST_FN,
  CHANGESERVINST_FN, CHECKACKS_FN, SENDPATHHBS_FN, SENDPATHHBHELPER_FN,
  CHECKHBACKS_FN, CHECKSERVACKS_FN,

  MAIN_FN,

  PRINTSTATS_FN,

  RECYCLEPATHID_FN, RECYCLEALTPATH_FN, RECYCLEREPAIR_FN,

  CHECKCPULOADLOG_FN,

  HANDLECTL_FN,
  HANDLECTLACK_FN,
  HANDLEDATA_FN,
  HANDLEDATAACK_FN,
  HANDLEDATAACKHELPER_FN,

  HANDLESOFTSTATE_FN,
  HANDLECREATE_FN,
  HANDLECSI_FN,
  HANDLEKILL_FN,

  SENDUPSTREAMINFO_FN,
  SENDHB_FN,
  SENDSOFTSTATE_FN,
  SENDDATAREQ_FN,

  CHECKHBSEND_FN,
  CHECKPATHTIMEOUT_FN,
  CHECKDUP_FN,
  CHECKDATASEND_FN,

  SPAWNTTSTHREAD_FN,
  FWDPKT_FN,

  PROCESSPKT_FN,

  SETINITIALACTIVE_FN,
  SETACTIVE_FN,
  SETINACTIVE_FN,
};

struct fn_type_name {
  enum fn_type type;
  char name[64];
};

struct patch_sub_s {
  short num_to_patch, num_in_bunch;
  short totLen;
  void init() {
    num_to_patch = num_in_bunch = -2;
    totLen = -2;
  }
};

struct ls_sub_s {
  short currRTT, lastRTT;
  void init() { currRTT = lastRTT = -2; }
};

struct emul_sub_s {
  long seedLong;
  long gap;
  unsigned long numSent; // the total number of pkts sent
  unsigned long numSentEmul; // the number of pkts sent with 'use_tunnel' flag
  unsigned long numRecd; // the total number of pkts recd
  unsigned long numRecdEmul; // the number of pkts recd with 'use_tunnel' flag
  unsigned long numBufOverflow; // the number of pkts discarded
  unsigned long numEmulDrop; // the number of pkts dropped in emulation
  unsigned long numInReadyQ; // the number of pkts in various ready queues
  unsigned long numInPQ; // the number of pkts in the priorityQ
  void init() {
    seedLong = -2; gap = -2;
    numSent = numSentEmul = 0;
    numRecd = numRecdEmul = 0;
    numBufOverflow = numEmulDrop = 0;
    numInReadyQ = numInPQ = 0;
  }
  static bool is_emul_desc(enum desc_type type) {
    return (type == emul_stats_desc);
  }
};

struct err_sub_s {
  short dup;
  struct in_addr peerIP;
  unsigned short curr_path_ver, curr_alt_ver;
  short lastHBPosn;
  void init() {
    dup = -2; peerIP.s_addr = (unsigned long)-2;
    curr_path_ver = curr_alt_ver = 0;
    lastHBPosn = -2;
  }
};

struct serv_sub_s {
  struct in_addr sIP;
  int sPort;
  void init() { sIP.s_addr = (unsigned long)-2; sPort = -2; }
};

enum hb_type_enum { INVALID_HB, HB_HB, KILL_HB };
struct hb_type_name { enum hb_type_enum type; char name[64]; };

enum app_type_enum { INVALID_APP, GEN_APP, TTS_SINK_APP, TTS_APP,
		     EMAIL_APP, NOOP_APP };
struct app_type_name { enum app_type_enum type; char name[64]; };

struct app_sub_s {
  int sc_listen_port;
  int data_port_base;

  int data_listen_port;

  struct in_addr destIP;
  int destPort;

  int currSentenceReq;
  int sentence_index;

  unsigned short serv_my_scid;
  unsigned short serv_dest_scid;
  unsigned short serv_upstream_scid;

  enum hb_type_enum hb_type;
  enum app_type_enum app_type;

  static unsigned short INVALID_SCID;

  void init() {
    sc_listen_port = data_port_base = data_listen_port = -2;
    destIP.s_addr = (unsigned long)-2; destPort = -2;
    currSentenceReq = sentence_index = -2;
    serv_my_scid = serv_dest_scid = serv_upstream_scid = INVALID_SCID;
    hb_type = INVALID_HB;
    app_type = INVALID_APP;
  }

  static bool is_app_desc(enum desc_type type) {
    if(type == app_listen_start_desc) { return true; }
    if(type == app_dup_req_desc) { return true; }
    if(type == app_no_free_cl_desc) { return true; }
    if(type == app_create_serv_desc) { return true; }
    if(type == app_change_serv_desc) { return true; }
    if(type == app_kill_serv_desc) { return true; }

    if(type == app_send_upstream_info_desc) { return true; }
    if(type == app_recv_upstream_info_desc) { return true; }
    if(type == app_ctl_ack_desc) { return true; }
    if(type == app_recv_hb_desc) { return true; }
    if(type == app_send_hb_desc) { return true; }
    if(type == app_path_timeout_desc) { return true; }
    if(type == app_path_kill_desc) { return true; }

    if(type == app_unknown_pkt_desc) { return true; }

    if(type == app_tts_req_desc) { return true; }
    if(type == app_data_ack_desc) { return true; }
    if(type == app_unknown_pkt_type_desc) { return true; }
    if(type == app_no_state_str_desc) { return true; }
    if(type == app_no_col_desc) { return true; }
    if(type == app_ok_desc) { return true; }
    if(type == app_ok_reorder_desc) { return true; }
    if(type == app_sc_reply_desc) { return true; }

    if(type == app_all_sentences_done_desc) { return true; }
    if(type == app_tts_resp_desc) { return true; }
    if(type == app_spawn_thread_desc) { return true; }
    if(type == app_thread_done_desc) { return true; }
    if(type == app_data_ack_timeout_desc) { return true; }
    if(type == app_send_soft_state_desc) { return true; }

    return false;
  }
};

enum u_type { INVALID_U, PATCH_U, LS_U, EMUL_U, ERR_U, SERV_U, APP_U };

enum log_key {
  INVALID_LK,
  FILE_LK,
  NEWSUBPATH_LK,
  DEAD_LK,
  SERVICES_LK, PHYSPATH_LK,
  SNAME_LK,

  STATE_STR_LK,
};

struct Log_KeyValuePair_LK {
  enum log_key key;
  JString value;
}; // End struct Log_KeyValuePair_LK
typedef struct Log_KeyValuePair_LK Log_KeyValuePair_LK_t;

struct Log_KeyValuePair {
  JString key;
  JString value;
}; // End struct Log_KeyValuePair
typedef struct Log_KeyValuePair Log_KeyValuePair_t;

/** Represents a line in the merged log-file */
struct LogLine {
  static unsigned short INVALID_SCID;
  static JString EMPTY_STR;

  static struct fn_type_name fn_type_name_map[];
  static enum fn_type get_fn_type(const JString &name);
  static const char *get_fn_name(enum fn_type type);

  static struct desc_type_name desc_type_name_map[];
  static enum desc_type get_desc_type(const JString &name);
  static const char *get_desc_name(enum desc_type type);

  static struct hb_type_name hb_type_name_map[];
  static enum hb_type_enum get_hb_type(const JString &name);
  static const char *get_hb_name(enum hb_type_enum type);

  static struct app_type_name app_type_name_map[];
  static enum app_type_enum get_app_type(const JString &name);
  static const char *get_app_name(enum app_type_enum type);

  bool succ;
  bool read_mode;

  unsigned long time;
  unsigned short scid;
  enum fn_type fn;
  enum desc_type desc;
  vector<Log_KeyValuePair_t> kv;
  vector<Log_KeyValuePair_LK_t> kv_lk;

  unsigned short dest_scid, origin_scid;
  unsigned short nextSCID, peerSCID;
  int pathID; unsigned short path_dest_scid;
  unsigned long duration;
  unsigned short version, fail_version;
  unsigned short path_origin_scid;
  unsigned short fail_dest_scid, fail_origin_scid;
  short failPosn;
  int pathCost;

  short posn, subPosn;

  unsigned long total_sent, total_recd;

  long cpu_load, edge_load;

  int success;
  enum u_type utype;

  union {
    struct patch_sub_s patch;
    struct ls_sub_s ls;
    struct emul_sub_s emul;
    struct err_sub_s err;
    struct serv_sub_s serv;
    struct app_sub_s app;
  } u;

  LogLine(); // init all values
  LogLine(const char *line); // parse from given line
  void init(); // init all values
  bool parse(const char *line); // parse from given line
  void err(const char *msg); // internal function
  JString toStr(); // convert back to JString
  JString printValid(); // print only the valid fields
  const JString &getValue(const JString &key) const; // get val corr to JString
  JString strip_getValue(const JString &key) const; // get val and strip []
  void addPair(enum log_key key, const JString &value);
}; // End class LogLine
typedef class LogLine LogLine_t;

#endif
