/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 19 October 2011
 *
 * Main entry point for client, gathers arguments, starts threads, and acts as
 * main controller for system
 */

/* Data for logging into server to be handled by send/receive system */

#ifndef MAIN
#define MAIN

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "thread.h"
#include "controller.h"
#include "keyboard.h"
#include "tick.h"
#include "ui.h"
#include "network.h"

typedef struct {
    bool anonymous;
    char *username;
    char *server;
    char *port;
} login_info;

#endif