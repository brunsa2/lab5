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

#define READ_BUFFER_SIZE 1024
#define MAX_TOKEN_SIZE 1024

#define fatal_shutdown(message) syslog(LOG_CRIT, message); \
        exit(EXIT_FAILURE);

typedef struct {
    int port;
    int max_threads;
    int send_time, send_time_idle, send_time_idle_after;
    int shutdown_delay;
    bool send_join_messages, send_leave_messages;
    bool shutdown_cancel;
} configuration;

static void init_configuration(configuration *config) {
    config->port = 65500;
    config->max_threads = 32;
    config->send_time = 600;
    config->send_time_idle = 900;
    config->send_time_idle_after = 600;
    config->shutdown_delay = 120;
    config->send_join_messages = true;
    config->send_leave_messages = true;
    config->shutdown_cancel = true;
}

configuration config;
bool should_terminate;

static void parse_configuration_line(configuration *config,
        char *configuration_line) {
    char identifier_token[MAX_TOKEN_SIZE];
    int value_token;
    
    if(sscanf(configuration_line, "%1023s %d", identifier_token,
            &value_token) < 2) {
        return;
    }
    
    if(strcmp(identifier_token, "port") == 0) {
        config->port = value_token;
    }
    
    if(strcmp(identifier_token, "max-threads") == 0) {
        config->max_threads = value_token;
    }
    
    if(strcmp(identifier_token, "send-time") == 0) {
        config->send_time = value_token;
    }
    
    if(strcmp(identifier_token, "send-time-idle") == 0) {
        config->send_time_idle = value_token;
    }
    
    if(strcmp(identifier_token, "send-time-idle-after") == 0) {
        config->send_time_idle_after = value_token;
    }
    
    if(strcmp(identifier_token, "send-join-messages") == 0) {
        config->send_join_messages = value_token == 1;
    }
    
    if(strcmp(identifier_token, "send-leave-messages") == 0) {
        config->send_leave_messages = value_token == 1;
    }
    
    if(strcmp(identifier_token, "shutdown-delay") == 0) {
        config->shutdown_delay = value_token;
    }
    
    if(strcmp(identifier_token, "shutdown-cancel") == 0) {
        config->shutdown_cancel = value_token == 1;
    }
}

static void read_configuration(configuration *config) {
    FILE *conf_fd;
    char conf_line[READ_BUFFER_SIZE];
    
    /* TODO: Check if file has right permissions */
    
    conf_fd = fopen("/etc/chat.conf", "r");
    if(conf_fd == NULL) {
        fatal_shutdown("Could not open configuration file");
    }
    
    while(fgets(conf_line, READ_BUFFER_SIZE, conf_fd) != NULL) {
        parse_configuration_line(config, conf_line);
    }
    
    if(fclose(conf_fd) != 0) {
        fatal_shutdown("Could not close configuration file");
    }
    
    syslog(LOG_DEBUG, "Port: %d. Max Threads: %d", config->port,
            config->max_threads);
    syslog(LOG_DEBUG, "Send time: %d unless no message in %d then %d",
            config->send_time, config->send_time_idle_after,
            config->send_time_idle);
    syslog(LOG_DEBUG, "Shutdown delay: %d", config->shutdown_delay);
    if(config->send_join_messages) {
        syslog(LOG_DEBUG, "Send join messages");
    }
    if(config->send_leave_messages) {
        syslog(LOG_DEBUG, "Send leave messages");
    }
    if(config->shutdown_cancel) {
        syslog(LOG_DEBUG, "Shutdown cancel enabled");
    }
}

static void chatd(void) {
    int x = 0;
    
    init_configuration(&config);
    read_configuration(&config);
    
    server();

}

void signal_handler(int signal) {
    switch(signal) {
        case SIGHUP:
            syslog(LOG_INFO, "chatd is refreshing");
            read_configuration(&config);
            break;
        case SIGTERM:
            should_terminate = true;
            syslog(LOG_INFO, "chatd is stopping");
            syslog(LOG_INFO, "chatd has stopped");
            exit(EXIT_SUCCESS);
    }
}

int main(int argc, char **argv) {
    become_daemon(signal_handler);
    chatd();
    return 0;
}