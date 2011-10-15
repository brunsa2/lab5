/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Daemon process that receives message and redistributes it to all connected
 * chat clients
 */

#include "chatd.h"

static void chatd(void) {
    int x = 0;
    
    for(;;) {
        sleep(10);
        syslog(LOG_DEBUG, "chatd is alive!");
    }

}

void signal_handler(int signal) {
    syslog(LOG_INFO, "Signal received");
}

int main(int argc, char **argv) {
    become_daemon(signal_handler);
    chatd();
    return 0;
}