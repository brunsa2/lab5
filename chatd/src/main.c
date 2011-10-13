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

#include "daemon.h"

int main(int argc, char **argv) {
    printf("Becoming daemon\n");
    become_daemon();
    
    for(;;) {
    }
}