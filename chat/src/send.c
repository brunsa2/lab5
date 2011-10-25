/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Network send thread
 */

#include "send.h"

void *send_thread(void *argument) {
    int sock_fd = (int) ((void *) argument);
    
    write(sock_fd,
          "Join\r\rUsername: brunsa2\r\n\r\n\r\n",
          strlen("Join\r\rUsername: brunsa2\r\n\r\n\r\n"));
    write(sock_fd,
          "Message\r\nUsername: brunsa2\r\n\r\nHi! How are you?\r\n\r\n",
          strlen("Message\r\nUsername: brunsa2\r\n\r\nHi! How are you?\r\n\r\n"));
    
    return NULL;
}

void send_message(message *sending_message) {
    
}