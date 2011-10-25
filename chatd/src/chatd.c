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
#define PORT_STR_LENGTH 8

#define fatal_shutdown(message) fprintf(stderr, "%s\n", message); \
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

int sock_fd;
struct linkedList thread_list;

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

static void chatd() {
    struct addrinfo hints, *servinfo, *current_addr;
    char port_str[PORT_STR_LENGTH];
    int gai_error_code;
    int yes = 1;
    int thread_index;
    thread_data new_thread;
    
    init_configuration(&config);
    read_configuration(&config);
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    sprintf(port_str, "%d", config.port);
    
    if((gai_error_code = getaddrinfo(NULL, port_str, &hints, &servinfo)) != 0) {
        syslog(LOG_CRIT, "Could not get address info: %s",
                gai_strerror(gai_error_code));
        exit(EXIT_FAILURE);
    }
    
    for(current_addr = servinfo; current_addr != NULL;
            current_addr = current_addr->ai_next) {
        if((sock_fd = socket(current_addr->ai_family, current_addr->ai_socktype,
                current_addr->ai_protocol)) == -1) {
            syslog(LOG_INFO, "Could not open socket with this address");
            continue;
        }
        
        if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                      sizeof(int)) == -1) {
            syslog(LOG_INFO, "Could not set up reusable addresses");
            exit(EXIT_FAILURE);
        }
        
        if(bind(sock_fd, current_addr->ai_addr, current_addr->ai_addrlen) ==
                -1) {
            close(sock_fd);
            syslog(LOG_INFO, "Could not bind to socket with this address");
            continue;
        }
        
        break;
    }
    
    if(current_addr == NULL) {
        syslog(LOG_CRIT, "Could not bind to socket");
        exit(EXIT_FAILURE);
    }
    
    freeaddrinfo(servinfo);
    
    if(listen(sock_fd, config.max_threads) == -1) {
        syslog(LOG_CRIT, "Could not listen");
        exit(EXIT_FAILURE);
    }
    
    ll_init(&thread_list);
    
    syslog(LOG_INFO, "Using socket fd %d", sock_fd);
    
    for(thread_index = 0; thread_index < config.max_threads; thread_index++) {
        thread_id thread;
        memset(&new_thread, 0, sizeof(new_thread));
        new_thread.socket = sock_fd;
        new_thread.should_shutdown = 0;
        new_thread.message_queue =
                (struct linkedList *) malloc(sizeof(struct linkedList));
        ll_init(new_thread.message_queue);
        ll_add(&thread_list, &new_thread, sizeof(new_thread));
        syslog(LOG_INFO, "Initialized thread %d", thread_index);
        start_thread(&thread, socket_thread,
                ll_get(&thread_list, thread_index));
    }
    
    for(;;) {
        sleep(10);
    }
}

void dispatch_message(message *dispatched_message) {
    struct linkedListIterator *iterator;
    iterator = ll_getIterator(&thread_list);
    while(ll_hasNext(iterator)) {
        thread_data *thread = ll_next(iterator);
        message *new_message;
        int line_index;
        
        new_message = (message *) malloc(sizeof(message));
        if(new_message == NULL) {
            continue;
        }
        new_message->header_size = dispatched_message->header_size;
        new_message->message_size = dispatched_message->message_size;
        new_message->headers = (char **) malloc(MAX_HEADERS * sizeof(char *));
        new_message->message = (char **) malloc(MAX_MESSAGES * sizeof(char *));
        
        for(line_index = 0; line_index < dispatched_message->header_size; line_index++) {
            new_message->headers[line_index] = (char *) malloc(strlen(dispatched_message->headers[line_index]));
            strcpy(new_message->headers[line_index], dispatched_message->headers[line_index]);
        }
        for(line_index = 0; line_index < dispatched_message->message_size; line_index++) {
            new_message->message[line_index] = (char *) malloc(strlen(dispatched_message->message[line_index]));
            strcpy(new_message->message[line_index], dispatched_message->message[line_index]);
        }
        
        ll_add(thread->message_queue, new_message, sizeof(message));
        
        free(new_message);
    }
    free(iterator);
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
            close(sock_fd);
            syslog(LOG_INFO, "chatd has stopped");
            exit(EXIT_SUCCESS);
    }
}

int main(int argc, char **argv) {
    become_daemon(signal_handler);
    chatd();
    return 0;
}