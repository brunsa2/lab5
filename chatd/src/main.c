/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Daemon process that receives message and redistributes it to all connected
 * chat clients
 */

#include <stdio.h>
#include <syslog.h>

#include "daemon.h"

int main(int argc, char **argv) {
    become_daemon();
    
    int x = 0;
    
    for(;;) {
        sleep(1);
        syslog(LOG_DEBUG, "chatd is alive!");
    }
}