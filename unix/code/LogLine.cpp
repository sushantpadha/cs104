/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
/** Implements LogLine */

#include <stdio.h>

#include "LogLine.h"
#include "../util/Util.h"

int print_ctl = 0;

unsigned short app_sub_s::INVALID_SCID = 0;
unsigned short LogLine::INVALID_SCID = 0;
JString LogLine::EMPTY_STR = "";

//---------- Function name definitions ----------//

const int num_fn_types = 55;
struct fn_type_name LogLine::fn_type_name_map[num_fn_types] = {
  { INVALID_FN, "INVALID" },

  { DEBUG_FN, "Debug" },

  { SENDTHREAD_FN, "sendThread" },
  { LISTENTHREAD_FN, "listenThread" },

  { PKTDROP_FN, "PktDrop" },

  { TIMEOUTPATH_FN, "timeOutPath" },
  { CHECKFAILURES_FN, "checkFailures" },
  { PATCHPATHSFROMPEER_FN, "patchPathsFromPeer" },
  { PATCHPATHSFROMPEERRC_FN, "patchPathsFromPeerRC" },
  { CHECKPATCHQ_FN, "checkPatchQ" },

  { PATCHPATH_FN, "patchPath" },
  { PATCHEDGELOCAL_FN, "patchEdgeLocal" },
  { PATCHEDGEE2EHELPER_FN, "patchEdgeE2EHelper" },
  { PEERPATCHPATHLOCAL_FN, "peerPatchPathLocal" },
  { PEERPATCHBUNCH_FN, "peerPatchBunch" },

  { PEERPATCHPATHE2E_FN, "peerPatchPathE2E" },
  { SENDBUNCH_FN, "sendBunch" },

  { CREATEPATH_FN, "createPath" },
  { CREATESERVINST_FN, "createServInst" },
  { PEERCREATESERVINST_FN, "peerCreateServInst" },
  { CHANGESERVINST_FN, "changeServInst" },
  { CHECKACKS_FN, "checkAcks" },

  { SENDPATHHBS_FN, "sendPathHBs" },
  { SENDPATHHBHELPER_FN, "sendPathHBHelper" },
  { CHECKHBACKS_FN, "checkHBAcks" },
  { CHECKSERVACKS_FN, "checkServAcks" },

  { MAIN_FN, "main" },

  { PRINTSTATS_FN, "printStats" },

  { RECYCLEPATHID_FN, "recyclePathID" },
  { RECYCLEALTPATH_FN, "recycleAltPath" },
  { RECYCLEREPAIR_FN, "recycleRepair" },

  { CHECKCPULOADLOG_FN, "checkCPULoadLog" },

  { HANDLECTL_FN,	"handleCtl" },
  { HANDLECTLACK_FN,	"handleCtlAck" },
  { HANDLEDATA_FN,	"handleData" },
  { HANDLEDATAACK_FN,	"handleDataAck" },
  { HANDLEDATAACKHELPER_FN,	"handleDataAckHelper" },

  { HANDLESOFTSTATE_FN,	"handleSoftState" },
  { HANDLECREATE_FN,	"handleCreate" },
  { HANDLECSI_FN,	"handleCSI" },
  { HANDLEKILL_FN,	"handleKill" },

  { SENDUPSTREAMINFO_FN,	"sendUpstreamInfo" },
  { SENDHB_FN,		"sendHB" },
  { SENDSOFTSTATE_FN,	"sendSoftState" },
  { SENDDATAREQ_FN,	"sendDataReq" },

  { CHECKHBSEND_FN,		"checkHBSend" },
  { CHECKPATHTIMEOUT_FN,	"checkPathTimeout" },
  { CHECKDUP_FN,		"checkDup" },
  { CHECKDATASEND_FN,		"checkDataSend" },

  { SPAWNTTSTHREAD_FN,	"spawnTTSThread" },
  { FWDPKT_FN,		"fwdPkt" },

  { PROCESSPKT_FN,	"processPkt" },

  { SETINITIALACTIVE_FN,	"setInitialActive" },
  { SETACTIVE_FN,		"setActive" },
  { SETINACTIVE_FN,		"setInactive" },
};

enum fn_type LogLine::get_fn_type(const JString &name) {
  int index = 0;
  for(int i = 0; i < num_fn_types; i++) {
    if(name == fn_type_name_map[i].name) {
      index = i; break;
    }
  }
  return fn_type_name_map[index].type;
} // End LogLine::get_fn_type()

const char *LogLine::get_fn_name(enum fn_type type) {
  int index = 0;
  for(int i = 0; i < num_fn_types; i++) {
    if(type == fn_type_name_map[i].type) {
      index = i; break;
    }
  }
  return fn_type_name_map[index].name;
} // End LogLine::get_fn_name()

//---------- Log description definitions ----------//

const int num_desc_types = 154;
struct desc_type_name LogLine::desc_type_name_map[num_desc_types] = {
  { invalid_desc,	"invalid" },

  { ack_for_sending_e2e_patch_towards_dest_desc,	"ack-for-sending-e2e-patch-towards-dest" },
  { add_path_desc,	"add-path" },
  { alt_done_desc,	"alt-done" },
  { alt_path_in_progress_desc,	"alt-path-in-progress" },
  { am_in_repaired_sub_path_desc,	"am-in-repaired-sub-path" },
  { am_last_in_sub_path_desc,	"am-last-in-sub-path" },
  { am_not_in_posn_desc,	"am-not-in-posn" },
  { am_not_in_sub_posn_desc,	"am-not-in-sub-posn" },
  { am_part_of_orig_path_desc,	"am-part-of-orig-path" },
  { am_upstream_of_sub_path_desc,	"am-upstream-of-sub-path" },
  { path_id_not_valid_desc,	"path-id-not-valid" },

  { bug_ack_from_non_peer_desc,	"bug-ack-from-non-peer" },
  { bunched_pkt_too_big_desc,	"bunched-pkt-too-big" },

  { ca_timeout_desc,	"ca-timeout" },
  { cp_done_desc,	"cp-done" },
  { cp_fail_desc,	"cp-fail" },
  { cp_req_desc,	"cp-req" },
  { cp_send_upstream_desc,	"cp-send-upstream" },
  { create_alt_path_desc,	"create-alt-path" },
  { creating_entry_for_path_desc,	"creating-entry-for-path" },
  { csi_req_desc,	"csi-req" },
  { cs_req_desc,	"cs-req" },

  { dead_links_desc,	"dead-links" },
  { del_non_existent_path_desc,	"del-non-existent-path" },
  { detect_time_desc,	"detect-time" },
  { drop_start_desc,	"drop-start" },
  { drop_end_desc,	"drop-end" },
  { dup_hb_desc,	"dup-hb" },
  { dup_req_desc,	"dup-req" },
  { dup_req_for_curr_version_desc,	"dup-req-for-curr-version" },

  { e2e_patch_desc,	"e2e-patch" },
  { e2e_req_desc,	"e2e-req" },
  { edge_repeat_try_e2e_desc,	"edge-repeat-try-e2e" },
  { err_alt_created_but_not_used_desc,	"err-alt-created-but-not-used" },
  { err_incomplete_alt_path_in_use_desc,	"err-incomplete-alt-path-in-use" },

  { graph_link_dead_desc,	"graph-link-dead" },
  { graph_link_live_desc,	"graph-link-live" },
  { graph_state_stale_desc,	"graph-state-stale" },

  { hb_ack_desc,	"hb-ack" },
  { hb_req_desc,	"hb-req" },

  { incoming_dead_desc,	"incoming-dead" },
  { incoming_restored_desc,	"incoming-restored" },
  { incomplete_path_desc,	"incomplete-path" },
  { invalid_num_in_bunch_desc,	"invalid-num-in-bunch" },
  { invalid_posn_desc,	"invalid-posn" },
  { invalid_sub_posn_desc,	"invalid-sub-posn" },

  { leave_for_downstream_desc,	"leave-for-downstream" },
  { local_exists_cant_handle_desc,	"local-exists-cant-handle" },
  { local_fail_try_e2e_desc,	"local-fail-try-e2e" },
  { lost_ack_desc,	"lost-ack" },
  { lost_ack_for_curr_version_desc,	"lost-ack-for-curr-version" },
  { lost_ack_for_old_version_desc,	"lost-ack-for-old-version" },
  { lost_ack_for_old_version_alt_in_construction_desc,	"lost-ack-for-old-version-alt-in-construction" },
  { lost_ack_for_past_version_desc,	"lost-ack-for-past-version" },
  { ls_update_from_unknown_peer_desc,	"ls-update-from-unknown-peer" },

  { new_sub_path_weird_desc,	"new-sub-path-weird" },
  { new_sub_path_weird_try_e2e_desc,	"new-sub-path-weird-try-e2e" },
  { node_repeat_try_e2e_desc,	"node-repeat-try-e2e" },
  { non_existent_edge_del_path_desc,	"non-existent-edge-del-path" },
  { non_existent_edge_ignoring_desc,	"non-existent-edge-ignoring" },
  { non_existent_path_desc,	"non-existent-path" },
  { no_phys_path_desc,	"no-phys-path" },
  { no_recognized_sub_path_desc,	"no-recognized-sub-path" },
  { no_replacement_try_e2e_desc,	"no-replacement-try-e2e" },
  { nothing_to_patch_desc,	"nothing-to-patch" },
  { num_in_bunch_desc,	"num-in-bunch" },

  { outgoing_dead_desc,	"outgoing-dead" },

  { parse_err_desc,	"parse-err" },
  { pass_ack_desc,	"pass-ack" },
  { pass_req_upstream_desc,	"pass-req-upstream" },
  { path_complete_desc,	"path-complete" },
  { path_ends_here_desc,	"path-ends-here" },
  { path_exists_here_desc,	"path-exists-here" },
  { path_not_found_desc,	"path-not-found" },
  { paths_unequal_desc,	"paths-unequal" },
  { pc_send_upstream_desc,	"pc-send-upstream" },
  { peer_cost_change_desc,	"peer-cost-change" },
  { peer_link_dead_desc,	"peer-link-dead" },
  { peer_link_live_desc,	"peer-link-live" },
  { phys_path_found_desc,	"phys-path-found" },
  { pkt_drop_desc,	"pkt-drop" },

  { rcv_fail_desc,	"rcv-fail" },
  { removing_old_path_hb_entry_desc,	"removing-old-path-hb-entry" },
  { repair_done_desc,	"repair-done" },
  { repair_exists_cant_handle_desc,	"repair-exists-cant-handle" },
  { repair_exists_try_e2e_desc,	"repair-exists-try-e2e" },
  { repair_not_in_db_desc,	"repair-not-in-db" },
  { replacement_found_desc,	"replacement-found" },
  { req_desc,	"req" },

  { sc_stopped_not_starting_recovery_desc,	"sc-stopped-not-starting-recovery" },
  { send_or_fwd_hb_desc,	"send-or-fwd-hb" },
  { send_or_fwd_hb_to_unknown_peer_desc,	"send-or-fwd-hb-to-unknown-peer" },
  { send_req_upstream_desc,	"send-req-upstream" },
  { stale_to_patch_desc,	"stale-to-patch" },
  { start_desc,	"start" },
  { start_dead_desc,	"start-dead" },
  { start_live_desc,	"start-live" },
  { sub_path_is_weird_desc,	"sub-path-is-weird" },

  { timing_out_path_desc,	"timing-out-path" },
  { timing_out_sub_path_desc,	"timing-out-sub-path" },
  { total_recd_desc,	"total-recd" },
  { total_sent_desc,	"total-sent" },
  { try_local_desc,	"try-local" },
  { try_patch_desc,	"try-patch" },

  { unequal_paths_desc,	"unequal-paths" },
  { unknown_peer_desc,	"unknown-peer" },
  { unknown_repair_path_desc,	"unknown-repair-path" },

  { version_mismatch_desc,	"version-mismatch" },

  { path_cost_desc,		"path-cost" },
  { repair_path_cost_desc,	"repair-path-cost" },
  { alt_path_cost_desc,		"alt-path-cost" },

  { emul_stats_desc,		"emul-stats" },
  { cpu_load_desc,		"cpu-load" },
  { edge_load_desc,		"edge-load" },

  { path_killed_before_alt_desc,	"path-killed-before-alt" },
  { path_killed_before_complete_desc,	"path-killed-before-complete" },
  { path_killed_before_repair_desc,	"path-killed-before-repair" },
  { path_killed_not_starting_recovery_desc,	"path-killed-not-starting-recovery" },
  { path_killed_before_serv_ack_desc,	"path-killed-before-serv-ack" },

  { err_decr_load_desc,		"err-decr-load" },

  { only_odd_path_recovery_desc,	"only-odd-path-recovery" },
  { log_limit_desc,			"log-limit" },

  { initial_active_desc,	"initial-active" },
  { set_active_desc,		"set-active" },
  { set_inactive_desc,		"set-inactive" },

  { app_listen_start_desc,	"listen-start" },
  { app_dup_req_desc,		"dup-req" },
  { app_no_free_cl_desc,	"no-free-cl" },
  { app_create_serv_desc,	"create-serv" },
  { app_change_serv_desc,	"change-serv" },
  { app_kill_serv_desc,		"kill-serv" },

  { app_send_upstream_info_desc,	"send-upstream-info" },
  { app_recv_upstream_info_desc,	"recv-upstream-info" },
  { app_ctl_ack_desc,		"ctl-ack" },
  { app_recv_hb_desc,		"recv-hb" },
  { app_send_hb_desc,		"send-hb" },
  { app_path_timeout_desc,	"path-timeout" },
  { app_path_kill_desc,		"path-kill" },

  { app_unknown_pkt_desc,	"unknown-pkt" },

  { app_tts_req_desc,		"tts-req" },
  { app_data_ack_desc,		"data-ack" },
  { app_unknown_pkt_type_desc,	"unknown-pkt-type" },
  { app_no_state_str_desc,	"no-state-str" },
  { app_no_col_desc,		"no-col" },
  { app_ok_desc,		"ok" },
  { app_ok_reorder_desc,	"ok-reorder" },
  { app_sc_reply_desc,		"sc-reply" },

  { app_all_sentences_done_desc,	"all-sentences-done" },
  { app_tts_resp_desc,		"tts-resp" },
  { app_spawn_thread_desc,	"spawn-thread" },
  { app_thread_done_desc,	"thread-done" },
  { app_data_ack_timeout_desc,	"data-ack-timeout" },
  { app_send_soft_state_desc,	"send-soft-state" },

};

enum desc_type LogLine::get_desc_type(const JString &name) {
  int index = 0;
  for(int i = 0; i < num_desc_types; i++) {
    if(name == desc_type_name_map[i].name) {
      index = i; break;
    }
  }
  return desc_type_name_map[index].type;
} // End LogLine::get_desc_type()

const char *LogLine::get_desc_name(enum desc_type type) {
  int index = 0;
  for(int i = 0; i < num_desc_types; i++) {
    if(type == desc_type_name_map[i].type) {
      index = i; break;
    }
  }
  return desc_type_name_map[index].name;
} // End LogLine::get_desc_name()

//---------- HB type definitions ----------//

const int num_hb_types = 3;
struct hb_type_name LogLine::hb_type_name_map[num_hb_types] = {
  { INVALID_HB,	"invalid" },
  { HB_HB,	"hb" },
  { KILL_HB,	"kill" },
};

enum hb_type_enum LogLine::get_hb_type(const JString &name) {
  int index = 0;
  for(int i = 0; i < num_hb_types; i++) {
    if(name == hb_type_name_map[i].name) {
      index = i; break;
    }
  }
  return hb_type_name_map[index].type;
} // End LogLine::get_hb_type()

const char *LogLine::get_hb_name(enum hb_type_enum type) {
  int index = 0;
  for(int i = 0; i < num_hb_types; i++) {
    if(type == hb_type_name_map[i].type) {
      index = i; break;
    }
  }
  return hb_type_name_map[index].name;
} // End LogLine::get_hb_name()

//---------- APP type definitions ----------//

const int num_app_types = 6;
struct app_type_name LogLine::app_type_name_map[num_app_types] = {
  { INVALID_APP,	"invalid" },
  { GEN_APP,		"generic" },
  { TTS_SINK_APP,	"tts_sink" },
  { TTS_APP,		"tts" },
  { EMAIL_APP,		"email" },
  { NOOP_APP,		"noop" },
};

enum app_type_enum LogLine::get_app_type(const JString &name) {
  int index = 0;
  for(int i = 0; i < num_app_types; i++) {
    if(name == app_type_name_map[i].name) {
      index = i; break;
    }
  }
  return app_type_name_map[index].type;
} // End LogLine::get_app_type()

const char *LogLine::get_app_name(enum app_type_enum type) {
  int index = 0;
  for(int i = 0; i < num_app_types; i++) {
    if(type == app_type_name_map[i].type) {
      index = i; break;
    }
  }
  return app_type_name_map[index].name;
} // End LogLine::get_app_name()

//---------- LogLine routines ----------//

LogLine::LogLine() {
  read_mode = false;
  init();
} // End LogLine::LogLine()

LogLine::LogLine(const char *line) {
  read_mode = true;
  parse(line);
} // End LogLine::LogLine(const char *)

void LogLine::init() {
  succ = true;

  time = 0;
  scid = INVALID_SCID;
  fn = INVALID_FN;
  desc = invalid_desc;
  while(kv.size() > 0) { kv.pop_back(); }

  dest_scid = origin_scid = path_dest_scid = path_origin_scid =
    fail_dest_scid = fail_origin_scid = INVALID_SCID;
  failPosn = -1;
  pathID = -1;
  duration = 0;
  version = fail_version = 0;
  nextSCID = peerSCID = INVALID_SCID;
  pathCost = -1;

  posn = subPosn = -1;

  total_recd = total_sent = 0;
  cpu_load = edge_load = -1;

  success = -2;
  utype = INVALID_U;
} // End LogLine::init()

bool LogLine::parse(const char *line) {
  if(!read_mode) {
    cerr << "parse called while not in read mode" << endl;
    exit(55);
  }

  init();

  // copy and pad an extra space at the end of the line
  char buf[2048];
  strcpy(buf, line);
  int len = strlen(line);
  if(buf[len-1] != ' ') { buf[len] = ' '; buf[len+1] = 0; len++; }
  char *p = buf;

  char *sp_pos1 = index(p, ' ');
  if(sp_pos1 == NULL) { err("sp_pos1"); return false; }
  *sp_pos1 = 0;
  time = JString_atoi(p); p = sp_pos1+1;

  char *sp_pos2 = index(p, ' ');
  if(sp_pos2 == NULL) { err("sp_pos2"); return false; }
  *sp_pos2 = 0;
  scid = JString_atoi(p); p = sp_pos2+1;

  char *sp_pos3 = index(p, ' ');
  if(sp_pos3 == NULL) { err("sp_pos3"); return false; }
  *sp_pos3 = 0;
  fn = get_fn_type(p); p = sp_pos3+1;

  char *sp_pos4 = index(p, ' ');
  if(sp_pos4 == NULL) { return true; }
  *sp_pos4 = 0;
  desc = get_desc_type(p); p = sp_pos4+1;

  // key-value pairs hence forth
  while(*p) {

    char *eq_posn = index(p, '=');
    if(eq_posn == NULL) { err("eq_posn"); return false; }
    *eq_posn = 0;
    char *key_posn = p, *val_posn = eq_posn+1;
    char *next_sp_posn;

    if(*val_posn == '[') {
      char *close_par_posn = index(val_posn, ']');
      if(close_par_posn == NULL) { err("unmatched-par"); return false; }
      next_sp_posn = close_par_posn+1;
      if(*next_sp_posn != ' ') {
	err("no-space-after-close-par"); return false;
      }
      *next_sp_posn = 0;
    } else {
      next_sp_posn = index(val_posn, ' ');
      if(next_sp_posn == NULL) {
	err("no-space-after-val"); return false;
      }
      *next_sp_posn = 0;
    }

    Log_KeyValuePair_t kvp = { key_posn, val_posn };
    kv.push_back(kvp);

    p = next_sp_posn+1;

  } // End while(reading all key-value pairs)

  if(emul_sub_s::is_emul_desc(desc)) { utype = EMUL_U; u.emul.init(); }
  if(app_sub_s::is_app_desc(desc)) { utype = APP_U; u.app.init(); }

  // get values for known keys
  for(unsigned int i = 0; i < kv.size(); i++) {
    if(kv[i].key == "dest_scid") {
      dest_scid = JString_atoi(kv[i].value); continue;
    }
    if(kv[i].key == "origin_scid") {
      origin_scid = JString_atoi(kv[i].value); continue;
    }

    if(kv[i].key == "peerSCID") {
      peerSCID = JString_atoi(kv[i].value); continue;
    }
    if(kv[i].key == "nextSCID") {
      nextSCID = JString_atoi(kv[i].value); continue;
    }

    if(kv[i].key == "p_id") {
      char buf[128];
      JString_cp(buf, kv[i].value);
      char *col_pos = index(buf, ':');
      if(col_pos == NULL) { err("p_id"); return false; }
      *col_pos = 0;
      pathID = atoi(buf); path_dest_scid = atoi(col_pos+1);
      continue;
    }

    if(kv[i].key == "fail") {
      char buf[128];
      JString_cp(buf, kv[i].value);
      char *sep_pos = index(buf, '-');
      if(sep_pos == NULL) { err("p_id"); return false; }
      *sep_pos = 0;
      fail_dest_scid = atoi(buf); fail_origin_scid = atoi(sep_pos+1);
      continue;
    }

    if(kv[i].key == "path_origin_scid") {
      path_origin_scid = JString_atoi(kv[i].value); continue;
    }
    if(kv[i].key == "duration") {
      duration = JString_atoi(kv[i].value); continue;
    }
    if(kv[i].key == "path_ver") {
      version = JString_atoi(kv[i].value); continue;
    }
    if(kv[i].key == "fail_path_ver") {
      fail_version = JString_atoi(kv[i].value); continue;
    }

    if(kv[i].key == "fail_dest_scid") {
      fail_dest_scid = JString_atoi(kv[i].value); continue;
    }
    if(kv[i].key == "fail_origin_scid") {
      fail_origin_scid = JString_atoi(kv[i].value); continue;
    }
    if(kv[i].key == "pathCost") {
      pathCost = JString_atoi(kv[i].value); continue;
    }

    if(kv[i].key == "total_sent") {
      total_sent = JString_atoi(kv[i].value); continue;
    }
    if(kv[i].key == "total_recd") {
      total_recd = JString_atoi(kv[i].value); continue;
    }

    if(kv[i].key == "cpu_load") {
      cpu_load = JString_atoi(kv[i].value); continue;
    }

    if(kv[i].key == "edge_load") {
      edge_load = JString_atoi(kv[i].value); continue;
    }

    if(emul_sub_s::is_emul_desc(desc)) {
      if(kv[i].key == "numSent") {
	u.emul.numSent = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "numSentEmul") {
	u.emul.numSentEmul = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "numRecd") {
	u.emul.numRecd = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "numRecdEmul") {
	u.emul.numRecdEmul = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "numBufOverflow") {
	u.emul.numBufOverflow = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "numEmulDrop") {
	u.emul.numEmulDrop = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "numInReadyQ") {
	u.emul.numInReadyQ = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "numInPQ") {
	u.emul.numInPQ = JString_atoi(kv[i].value); continue;
      }

    } // End if(emul_sub_s::is_emul_desc(desc))

    if(app_sub_s::is_app_desc(desc)) {
      if(kv[i].key == "sc_listen_port") {
	u.app.sc_listen_port = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "data_port_base") {
	u.app.data_port_base = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "data_listen_port") {
	u.app.data_listen_port = JString_atoi(kv[i].value); continue;
      }

      if(kv[i].key == "dest") {
	parseIPPort(kv[i].value.cstr(), &u.app.destIP, &u.app.destPort); continue;
      }

      if(kv[i].key == "currSentenceReq") {
	u.app.currSentenceReq = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "sentence_index") {
	u.app.sentence_index = JString_atoi(kv[i].value); continue;
      }

      if(kv[i].key == "serv_my_scid") {
	u.app.serv_my_scid = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "serv_dest_scid") {
	u.app.serv_dest_scid = JString_atoi(kv[i].value); continue;
      }
      if(kv[i].key == "serv_upstream_scid") {
	u.app.serv_upstream_scid = JString_atoi(kv[i].value); continue;
      }

      if(kv[i].key == "type") {
	u.app.hb_type = get_hb_type(kv[i].value); continue;
      }
      if(kv[i].key == "name") {
	u.app.app_type = get_app_type(kv[i].value); continue;
      }

    } // End if(app_sub_s::is_app_desc(desc))

  } // End looping through all key-value pairs

  // if optimizing on space, can discard the other info
  if(Cfg.cfg_opt_logline) { while(kv.size() > 0) { kv.pop_back(); } }

  return true;
} // End LogLine::parse()

void LogLine::err(const char *msg) {
  fprintf(stderr, "%s\n", msg); succ = false;
} // End LogLine::err()

JString LogLine::toStr() {
  if(!read_mode) {
    cerr << "toStr called while not in read mode" << endl;
    exit(55);
  }

  JString toRet = long2str(time) + " " + long2str(scid) + " " +
    get_fn_name(fn) + " " + get_desc_name(desc);
  for(unsigned int i = 0; i < kv.size(); i++) {
    toRet += " " + kv[i].key + "=" + kv[i].value;
  }
  return toRet;
} // End LogLine::toStr()

JString LogLine::printValid() {
  if(read_mode) {
    cerr << "printValid called while in read mode" << endl;
    exit(55);
  }

  JString toRet = long2str(time) + " " + long2str(scid) + " " +
    get_fn_name(fn) + " " + get_desc_name(desc);

  if(pathID != -1) {
    toRet += " p_id=" + long2str(pathID) + ":" + long2str(path_dest_scid);
  }
  if(path_origin_scid != INVALID_SCID) {
    toRet += " path_origin_scid=" + long2str(path_origin_scid);
  }
  if(duration != 0) {
    toRet += " duration=" + long2str(duration);
  }
  if(version != 0) {
    toRet += " path_ver=" + long2str(version);
  }

  if(dest_scid != INVALID_SCID) {
    toRet += " dest_scid=" + long2str(dest_scid);
  }
  if(origin_scid != INVALID_SCID) {
    toRet += " origin_scid=" + long2str(origin_scid);
  }

  if(peerSCID != INVALID_SCID) {
    toRet += " peerSCID=" + long2str(peerSCID);
  }
  if(nextSCID != INVALID_SCID) {
    toRet += " nextSCID=" + long2str(nextSCID);
  }

  if(fail_dest_scid != INVALID_SCID) {
    toRet += " fail_dest_scid=" + long2str(fail_dest_scid);
  }
  if(fail_origin_scid != INVALID_SCID) {
    toRet += " fail_origin_scid=" + long2str(fail_origin_scid);
  }
  if(failPosn != -1) {
    toRet += " failPosn=" + long2str(failPosn);
  }
  if(fail_version != 0) {
    toRet += " fail_path_ver=" + long2str(fail_version);
  }
  if(pathCost != -1) {
    toRet += " pathCost=" + long2str(pathCost);
  }

  if(total_recd != 0) {
    toRet += " total_recd=" + long2str(total_recd);
  }
  if(total_sent != 0) {
    toRet += " total_sent=" + long2str(total_sent);
  }

  if(cpu_load != -1) {
    toRet += " cpu_load=" + long2str(cpu_load);
  }

  if(edge_load != -1) {
    toRet += " edge_load=" + long2str(edge_load);
  }

  if(posn != -1) {
    toRet += " posn=" + long2str(posn);
  }
  if(subPosn != -1) {
    toRet += " subPosn=" + long2str(subPosn);
  }

  if(success != -2) {
    toRet += " success=" + long2str(success);
  }

  if(utype == PATCH_U) {
    if(u.patch.num_to_patch != -2) {
      toRet += " num_to_patch=" + long2str(u.patch.num_to_patch);
    }
    if(u.patch.num_in_bunch != -2) {
      toRet += " num_in_bunch=" + long2str(u.patch.num_in_bunch);
    }
    if(u.patch.totLen != -2) {
      toRet += " totLen=" + long2str(u.patch.totLen);
    }

    // End case PATCH_U
  } else if(utype == LS_U) {
    if(u.ls.currRTT != -2) {
      toRet += " currRTT=" + long2str(u.ls.currRTT);
    }
    if(u.ls.lastRTT != -2) {
      toRet += " lastRTT=" + long2str(u.ls.lastRTT);
    }

    // End case LS_U
  } else if(utype == EMUL_U) {
    if(u.emul.seedLong != -2) {
      toRet += " seedLong=" + long2str(u.emul.seedLong);
    }
    if(u.emul.gap != -2) {
      toRet += " gap=" + long2str(u.emul.gap);
    }

    if(u.emul.numSent != 0) {
      toRet += " numSent=" + long2str(u.emul.numSent);
    }
    if(u.emul.numSentEmul != 0) {
      toRet += " numSentEmul=" + long2str(u.emul.numSentEmul);
    }
    if(u.emul.numRecd != 0) {
      toRet += " numRecd=" + long2str(u.emul.numRecd);
    }
    if(u.emul.numRecdEmul != 0) {
      toRet += " numRecdEmul=" + long2str(u.emul.numRecdEmul);
    }
    if(u.emul.numBufOverflow != 0) {
      toRet += " numBufOverflow=" + long2str(u.emul.numBufOverflow);
    }
    if(u.emul.numEmulDrop != 0) {
      toRet += " numEmulDrop=" + long2str(u.emul.numEmulDrop);
    }
    if(u.emul.numInReadyQ != 0) {
      toRet += " numInReadyQ=" + long2str(u.emul.numInReadyQ);
    }
    if(u.emul.numInPQ != 0) {
      toRet += " numInPQ=" + long2str(u.emul.numInPQ);
    }

    // End case EMUL_U
  } else if(utype == ERR_U) {
    if(u.err.dup != -2) {
      toRet += " dup=" + long2str(u.err.dup);
    }
    if(u.err.peerIP.s_addr != (unsigned long)-2) {
      toRet += " peerIP=" + ip2str(u.err.peerIP);
    }
    if(u.err.curr_path_ver != 0) {
      toRet += " curr_path_ver=" + long2str(u.err.curr_path_ver);
    }
    if(u.err.curr_alt_ver != 0) {
      toRet += " curr_alt_ver=" + long2str(u.err.curr_alt_ver);
    }
    if(u.err.lastHBPosn != -2) {
      toRet += " lastHBPosn=" + long2str(u.err.lastHBPosn);
    }


    // End case ERR_U
  } else if(utype == SERV_U) {
    if(u.serv.sIP.s_addr != (unsigned long)-2) {
      toRet += " sIP=" + ip2str(u.serv.sIP);
    }
    if(u.serv.sPort != -2) {
      toRet += " sPort=" + long2str(u.serv.sPort);
    }

    // End case SERV_U
  } else if(utype == APP_U) {
    if(u.app.sc_listen_port != -2) {
      toRet += " sc_listen_port=" + long2str(u.app.sc_listen_port);
    }
    if(u.app.data_port_base != -2) {
      toRet += " data_port_base=" + long2str(u.app.data_port_base);
    }
    if(u.app.data_listen_port != -2) {
      toRet += " data_listen_port=" + long2str(u.app.data_listen_port);
    }

    if(u.app.destIP.s_addr != (unsigned long)-2) {
      toRet += " dest=" + ip2str(u.app.destIP) +
	":" + long2str(u.app.destPort);
    }

    if(u.app.currSentenceReq != -2) {
      toRet += " currSentenceReq=" + long2str(u.app.currSentenceReq);
    }
    if(u.app.sentence_index != -2) {
      toRet += " sentence_index=" + long2str(u.app.sentence_index);
    }

    if(u.app.serv_my_scid != INVALID_SCID) {
      toRet += " serv_my_scid=" + long2str(u.app.serv_my_scid);
    }
    if(u.app.serv_dest_scid != INVALID_SCID) {
      toRet += " serv_dest_scid=" + long2str(u.app.serv_dest_scid);
    }
    if(u.app.serv_upstream_scid != INVALID_SCID) {
      toRet += " serv_upstream_scid=" + long2str(u.app.serv_upstream_scid);
    }

    if(u.app.hb_type != INVALID_HB) {
      toRet += " type=" + JString(get_hb_name(u.app.hb_type));
    }
    if(u.app.app_type != INVALID_APP) {
      toRet += " name=" + JString(get_app_name(u.app.app_type));
    }

    // End case APP_U
  }

  for(unsigned int i = 0; i < kv_lk.size(); i++) {
    JString keyStr = "INVALID_LK";
    if(kv_lk[i].key == INVALID_LK) { keyStr = "INVALID_LK"; }
    else if(kv_lk[i].key == FILE_LK) { keyStr = "file"; }
    else if(kv_lk[i].key == NEWSUBPATH_LK) { keyStr = "newSubPath"; }
    else if(kv_lk[i].key == DEAD_LK) { keyStr = "dead"; }
    else if(kv_lk[i].key == SERVICES_LK) { keyStr = "services"; }
    else if(kv_lk[i].key == PHYSPATH_LK) { keyStr = "physPath"; }
    else if(kv_lk[i].key == SNAME_LK) { keyStr = "sName"; }
    else if(kv_lk[i].key == STATE_STR_LK) { keyStr = "state_str"; }

    toRet += " " + keyStr + "=" + kv_lk[i].value;
  }

  return toRet;
} // End LogLine::printValid()

const JString &LogLine::getValue(const JString &key) const {
  if(!read_mode) {
    cerr << "getValue called while not in read mode" << endl;
    exit(55);
  }
  for(unsigned int i = 0; i < kv.size(); i++) {
    if(kv[i].key == key) return kv[i].value;
  }
  return EMPTY_STR;
} // End LogLine::getValue()

JString LogLine::strip_getValue(const JString &key) const {
  if(!read_mode) {
    cerr << "strip_getValue called while not in read mode" << endl;
    exit(55);
  }

  const JString &val1 = getValue(key);
  char buf[1024];
  JString_cp(buf, val1);
  int len = strlen(buf);
  buf[0] = 0; buf[len-1] = 0; // mark off the '[' and ']'
  return (char*)(buf+1);
} // End LogLine::strip_getValue()

void LogLine::addPair(enum log_key key, const JString &value) {
  if(read_mode) {
    cerr << "addPair called while in read mode" << endl;
    exit(55);
  }

  vector<Log_KeyValuePair_LK_t>::iterator iter;
  int found = 0;
  for(iter = kv_lk.begin(); iter != kv_lk.end(); iter++) {
    if(iter->key == key) {
      found = 1; break;
    }
  }

  if(found) {
    iter->value = value;
  } else {
    Log_KeyValuePair_LK_t kv_pair = { key, value };
    kv_lk.push_back(kv_pair);
  }
} // End LogLine::addPair()
