/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Chat daemon control
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PID_STRING_LENGTH 16

#define fatal_shutdown(message) fprintf(stderr, "%s\n", message); \
        exit(EXIT_FAILURE);

static void print_usage_and_exit_with_code(int exit_status) {
    fprintf(stderr, "Usage: chatctl command\n\n"
            "start\t\tStart chatd\n"
            "stop\t\tStop chatd\n"
            "restart\t\tRestard chatd\n"
            "refresh\t\tRefresh server and cancel any server shutdown\n"
            "force-stop\tForce restart chatd\n"
            "force-restart\tForce restart chatd\n\n");
    exit(exit_status);
}

static void send_signal_to_chatd(int signal) {
    int lock_fd, pid_error;
    char pid_string[PID_STRING_LENGTH];
    pid_t pid;
    
    lock_fd = open("./chatd.lock", O_RDONLY);
    if(lock_fd < 0) {
        fatal_shutdown("Could not open lock file");
    }
    if(read(lock_fd, pid_string, PID_STRING_LENGTH) < 1) {
        fatal_shutdown("Could not read lock file");
    }
    close(lock_fd);
    
    pid = (pid_t) atoi(pid_string);
    pid_error = kill(pid, signal);
    if(pid_error != 0) {
        fatal_shutdown("Could not send signal to chatd");
    }
}

static void start_chatd(void) {
    printf("Starting chatd...\n");
    execl("./chatd", "./chatd", (char *) NULL);
    printf("Could not start chatd");
    exit(EXIT_FAILURE);
}

static void stop_chatd(void) {
    printf("Stopping chatd...\n");
    send_signal_to_chatd(SIGTERM);
}

static void refresh_chatd(void) {
    printf("Refreshing chatd...\n");
    send_signal_to_chatd(SIGHUP);
}

static void force_stop_chatd(void) {
    printf("Force stopping chatd...\n");
    send_signal_to_chatd(SIGKILL);
}

int main(int argc, char **argv) {    
    if(argc > 1 &&
            (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        print_usage_and_exit_with_code(EXIT_SUCCESS);
    }
    
    if(strcmp(argv[0], "chatd-start") == 0 ||
            (argc > 1 && strcmp(argv[1], "start") == 0)) {
        start_chatd();
        return 0;
    }
    
    if(strcmp(argv[0], "chatd-stop") == 0 ||
       (argc > 1 && strcmp(argv[1], "stop") == 0)) {
        stop_chatd();
        return 0;
    }
    
    if(strcmp(argv[0], "chatd-restart") == 0 ||
       (argc > 1 && strcmp(argv[1], "restart") == 0)) {
        stop_chatd();
        start_chatd();
        return 0;
    }
    
    if(strcmp(argv[0], "chatd-refresh") == 0 ||
       (argc > 1 && strcmp(argv[1], "refresh") == 0)) {
        refresh_chatd();
        return 0;
    }
    
    if(strcmp(argv[0], "chatd-force-stop") == 0 ||
       (argc > 1 && strcmp(argv[1], "force-stop") == 0)) {
        force_stop_chatd();
        return 0;
    }
    
    if(strcmp(argv[0], "chatd-force-restart") == 0 ||
       (argc > 1 && strcmp(argv[1], "force-restart") == 0)) {
        force_stop_chatd();
        start_chatd();
        return 0;
    }
    
    print_usage_and_exit_with_code(EXIT_FAILURE);
}