/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
#ifndef CIRC_QUEUE_H
#define CIRC_QUEUE_H

#include <stdio.h>

/** Defines a circular queue template */

template <class EltType_t>
class CircQueue {
protected:
  /** The first valid elt, if it exists */
  unsigned int head;
  /** The elt next to the last valid elt. NOTE: head==tail signifies
      empty queue */
  unsigned int tail;
  /** The array of elements */
  EltType_t *arr;
  /** The size of the array */
  unsigned int arr_size;

  /** The circular successor index */
  inline unsigned int circ_succ(unsigned int i) {
    return (i == (arr_size-1)) ? 0 : (i+1);
  }
  /** The circular predecessor index */
  inline unsigned int circ_pred(unsigned int i) {
    return (i == 0) ? (arr_size-1) : (i-1);
  }
  /** Circular addition */
  inline unsigned int circ_add(unsigned int i, unsigned int x) {
    unsigned int ind = i+x;
    return (ind < arr_size) ? ind : (ind - arr_size);
  }

public:
  /** The constructor */
  CircQueue(unsigned int numElts, bool init_with_full) {
    arr_size = numElts;
    arr = new EltType_t[arr_size];
    if(arr == NULL) {
      fprintf(stderr, "CircQueue: no space for arr\n");
      exit(135);
    }
    head = tail = 0;
    if(init_with_full) { tail = arr_size-1; }
  }
  /** The destructor */
  ~CircQueue() { delete[] arr; }

  /** Return the elt at head; returns NULL on failure */
  inline EltType_t *head_elt() { return empty() ? NULL : &arr[head]; }
  /** Return the elt at the tail; returns NULL on failure */
  inline EltType_t *tail_elt() { return empty() ? NULL : &arr[circ_pred(tail)]; }
  /** Return the elt at given position (head is position 0); returns
      NULL on failure */
  inline EltType_t *get_elt_at(unsigned int index) {
    return (index < num_elts()) ? &arr[circ_add(head, index)] : NULL;
  }

  /** Is circ queue empty? */
  inline bool empty() { return (head == tail); }
  /** Is circ queue full? */
  inline bool full() { return (head == circ_succ(tail)); }

  /** Return the number of elements currently in the queue */
  inline unsigned int num_elts() {
    return (head <= tail) ? (tail - head) : (tail + arr_size - head);
  }

  /** Remove the elt at the head, return success */
  inline bool remove_head() {
    if(empty()) { return false; }
    head = circ_succ(head); return true;
  }
  /** Add an elt at the tail, return success */
  inline bool add_at_tail() {
    if(full()) { return false; }
    tail = circ_succ(tail); return true;
  }
  /** Rotate the queue by one elt */
  inline void rotate() { head = circ_succ(head); tail = circ_succ(tail); }

}; // End class CircQueue

#endif
