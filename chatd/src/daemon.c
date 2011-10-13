/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Daemonizing function to start server running
 */

#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "daemon.h"

int become_daemon(void) {
    int maximum_fd, current_fd;
    
    /* Become a background process */
    switch(fork()) {
        case -1:
            return -1;
        case 0:
            /* Child process should continue */
            break;
        default:
            _exit(EXIT_SUCCESS);
    }
    
    /* Become leader of new session */
    if(setsid() == -1) {
        return -1;
    }
    
    /* Ensure that we are not leader of new session */
    switch(fork()) {
        case -1:
            return -1;
        case 0:
            /* Child process should continue */
            break;
        default:
            _exit(EXIT_SUCCESS);
    }
    
    /* Clear file mode creation mask and chdir to root */
    umask(0);
    chdir("/");
    
    /* Close all files */
    maximum_fd = sysconf(_SC_OPEN_MAX);
    if(maximum_fd == -1) {
        maximum_fd = MAX_FILE_DESCRIPTOR_TO_CLOSE;
    }
    for(current_fd = 0; current_fd < maximum_fd; current_fd++) {
        close(current_fd);
    }
    
    /* std*** to /dev/null */
    close(STDIN_FILENO);
    
    current_fd = open("/dev/null", O_RDWR);
    
    if(current_fd != STDIN_FILENO) {
        return -1;
    }
    if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO) {
        return -1;
    }
    if(dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO) {
        return -1;
    }
    
    return 0;
}