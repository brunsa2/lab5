/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Definitions and prototypes for chat daemon
 */

#ifndef CHATD
#define CHATD

#define MAX_HEADER_SIZE 16
#define MAX_MESSAGE_SIZE 16

#include <stdio.h>
#include <syslog.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include "daemon.h"
#include "linkedlist.h"
#include "thread.h"

typedef struct {
    int socket;
    struct linkedList message_queue;
} thread_data;

typedef struct {
    char *headers[MAX_HEADER_SIZE];
    int header_size;
    char *message[MAX_MESSAGE_SIZE];
    int message_size;
} message;

#include "socketthread.h"

void signal_handler(int);

#endif