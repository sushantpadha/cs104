#ifndef KEY_VALUE_H
#define KEY_VALUE_H

#include "JString.h"
#include <vector>

class KeyValue {

protected:
  struct kv {
    JString key;
    JString value;
  };
  typedef struct kv kv_t;

  vector<kv_t> kv_vec;

public:
  // Default constructor
  KeyValue() {}
  // Constructor from string
  KeyValue(const char *s) { read_kv(s, ','); }
  // Read key-value pairs from string 's' with given separator
  void read_kv(const char *s, char sep);
  // Read key-value pairs from a file
  void read_kv_file(const char *fName);
  // Add a key-value pair
  void add_kv_pair(const char *key, const char *value);
  // Get the value corresponding to the given key, NULL if no such key
  const char *get_value(const char *key);
  // Check if the given key exists
  bool keyExists(const char *key) { return (get_value(key) != NULL); }

}; // End class KeyValue
typedef class KeyValue KeyValue_t;

#endif
