/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Daemonizing function to start server running
 */

#include "daemon.h"

#define PID_STRING_LENGTH 16

#define fatal_shutdown(message) syslog(LOG_CRIT, message); \
        exit(EXIT_FAILURE);
#define nonfatal_shutdown(message) syslog(LOG_NOTICE, message); \
        exit(EXIT_SUCCESS);

int become_daemon(void) {
    int maximum_fd, current_fd, lock_fd;
    char pid_string[PID_STRING_LENGTH];
    
    openlog("chatd", LOG_CONS, LOG_USER);
    
    syslog(LOG_INFO, "This is chatd. 14 October 2011 Jeff Stubler for CS 3841 "
            "UNIX chat system lab 5");
    
    /* Become a background process */
    switch(fork()) {
        case -1:
            fatal_shutdown("Error becoming background process");
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
            fatal_shutdown("Error breaking from leadership of new session");
        case 0:
            /* Child process should continue */
            break;
        default:
            _exit(EXIT_SUCCESS);
    }
    
    syslog(LOG_INFO, "chatd is now running in the background");
    
    /* Clear file mode creation mask and chdir to root */
    umask(0027);
    //chdir("/");
    
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
        fatal_shutdown("Cannot attach stdin to /dev/null");
    }
    if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO) {
        fatal_shutdown("Cannot attach stdout to /dev/null");
    }
    if(dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO) {
        fatal_shutdown("Cannot attach stderr to /dev/null");
    }
    
    syslog(LOG_INFO, "chatd is ready to lock system");
    
    /* Create lock file */
    lock_fd = open("chatd.lock", O_RDWR | O_CREAT, 0640);
    if(lock_fd < 0) {
        fatal_shutdown("Cannot open lock file");
    }
    if(lockf(lock_fd, F_TLOCK, 0) < 0) {
        nonfatal_shutdown("chatd is already running---this chatd will stop");
    }
    
    sprintf(pid_string, "%d\n", getpid());
    write(lock_fd, pid_string, strlen(pid_string));
    
    syslog(LOG_INFO, "chatd is locked");
    syslog(LOG_INFO, "chatd has started successfully");
    
    return 0;
}