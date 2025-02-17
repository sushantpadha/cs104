#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>

#include "KeyValue.h"

void KeyValue::add_kv_pair(const char *key, const char *value) {
  for(unsigned int i = 0; i < kv_vec.size(); i++) {
    if(kv_vec[i].key == key) {
      kv_vec[i].value = value; return;
    }
  }
  kv_t to_add;
  to_add.key = key; to_add.value = value;
  kv_vec.push_back(to_add);
} // End KeyValue::add_kv_pair()

const char *KeyValue::get_value(const char *key) {
  for(unsigned int i = 0; i < kv_vec.size(); i++) {
    if(kv_vec[i].key == key) {
      return kv_vec[i].value.cstr();
    }
  }
  return NULL;
} // End KeyValue::get_value()

void KeyValue::read_kv(const char *s, char sep) {
  char buf[8192];
  strcpy(buf, s);
  char *s1 = buf;

  while(*s1 != 0) {
    char *sep_pos = index(s1, sep);
    if(sep_pos != NULL) { *sep_pos = 0; }

    char *eq_pos = index(s1, '=');
    assert(eq_pos != NULL);
    *eq_pos = 0;

    char *key = s1;
    char *value = eq_pos+1;
    add_kv_pair(key, value);

    if(sep_pos == NULL) { break; }
    else { s1 = sep_pos+1; }
  }
} // End KeyValue::read_kv()

void KeyValue::read_kv_file(const char *fName) {

  ifstream fin(fName);
  if(fin.fail()) {
    cerr << "Cannot open config file " << fName << endl;
    exit(98);
  }

  for(;;) {
    // read each line
    if(fin.eof()) break;
    char line[1024];
    fin.getline(line, sizeof(line)); // read a line
    if(fin.eof()) break;
    if(fin.fail()) {
      cerr << "Error reading " << fName << endl;
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

    // Add the key-value pair
    const char *key = line_start;
    const char *val = (eq_pos+1);
    add_kv_pair(key, val);

  } // End for(;;) reading all lines in config file

  fin.close();

} // End KeyValue::read_kv_file()
