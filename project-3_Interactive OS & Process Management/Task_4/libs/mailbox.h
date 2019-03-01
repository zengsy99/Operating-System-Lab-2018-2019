#ifndef INCLUDE_MAIL_BOX_
#define INCLUDE_MAIL_BOX_

#define MAX_NUM_BOX 32
#define MAX_BUFFER_LENGTH 64
#define MAX_NAME_LENGTH 32

#include "cond.h"

typedef struct mailbox
{
  char name[MAX_NAME_LENGTH];
  char buffer [MAX_BUFFER_LENGTH];
  int user_num;

  int num_item;
  int read_index;
  int write_index;

  condition_t not_empty;
  condition_t not_full;
} mailbox_t;


void do_mbox_init();
mailbox_t *do_mbox_open(char *);
void do_mbox_close(mailbox_t *);
void do_mbox_send(mailbox_t *, void *, int);
void do_mbox_recv(mailbox_t *, void *, int);

#endif
