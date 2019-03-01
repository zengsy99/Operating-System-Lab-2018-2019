#include "string.h"
#include "mailbox.h"

static mailbox_t mboxs[MAX_NUM_BOX];
static mutex_lock_t mutex;

void do_mbox_init()
{
  int i;
  do_mutex_lock_init(&mutex);

  for(i = 0; i < MAX_NUM_BOX; i++)
  {
    mboxs[i].name[0] = '\0';
    mboxs[i].user_num = 0;
    mboxs[i].num_item = 0;
    mboxs[i].read_index = 0;
    mboxs[i].write_index = 0;
    bzero( (void *)mboxs[i].buffer,  MAX_BUFFER_LENGTH);

    do_condition_init( &mboxs[i].not_empty );
    do_condition_init( &mboxs[i].not_full );
  }

}

mailbox_t *do_mbox_open(char *name)
{
  int i;

  // return the exited mbox
  for(i = 0; i < MAX_NUM_BOX; i++)
  {
    if( strcmp(mboxs[i].name, name) == 0 && mboxs[i].user_num != 0)
    {
      mboxs[i].user_num++;
      return &mboxs[i];
    }
  }

  // return a new mbox
  for(i = 0; i < MAX_NUM_BOX; i++)
  {
    if( mboxs[i].user_num == 0 )
    {
      memcpy(mboxs[i].name, name ,strlen(name));
      mboxs[i].user_num++;
      return &mboxs[i];
    }
  }
}

void do_mbox_close(mailbox_t *mailbox)
{
  if(mailbox->user_num == 0)
  {
    mailbox->name[0] = '\0';
    mailbox->user_num = 0;
    mailbox->num_item = 0;
    mailbox->read_index = 0;
    mailbox->write_index = 0;
    bzero( (void *)mailbox->buffer,  MAX_BUFFER_LENGTH);

    do_condition_init( &(mailbox->not_empty) );
    do_condition_init( &(mailbox->not_full) );
  }
}

void do_mbox_send(mailbox_t *mailbox, void *msg, int msg_length)
{
  int i;
  do_mutex_lock_acquire(&mutex);
  
  for(i = 0; i < msg_length; i++)
  {
    while(mailbox->num_item == MAX_BUFFER_LENGTH)
      do_condition_wait(&mutex, &(mailbox->not_full));

    mailbox->buffer[mailbox->write_index++] = *((char *)msg + i);
    mailbox->write_index %= MAX_BUFFER_LENGTH;
    mailbox->num_item++;
  }

  do_mutex_lock_release(&mutex);

  // do_condition_signal(&(mailbox->not_empty));
  do_condition_broadcast(&(mailbox->not_empty));

}

void do_mbox_recv(mailbox_t *mailbox, void *msg, int msg_length)
{
  int i;
  do_mutex_lock_acquire(&mutex);
  
  for(i = 0; i < msg_length; i++)
  {
    while(mailbox->num_item == 0)
      do_condition_wait(&mutex, &(mailbox->not_empty));

    *((char *)msg + i) = mailbox->buffer[mailbox->read_index++];
    mailbox->read_index %= MAX_BUFFER_LENGTH;
    mailbox->num_item--;
  }
  
  do_mutex_lock_release(&mutex);

  // do_condition_signal(&(mailbox->not_full));
  do_condition_broadcast(&(mailbox->not_full));
}
