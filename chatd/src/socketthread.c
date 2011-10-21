/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 15 October 2011
 *
 * Main thread for each socket
 */

#include "socketthread.h"

#define BUFFER_SIZE 128

extern int errno;

mutex_id accept_lock = PTHREAD_MUTEX_INITIALIZER;

static int read_line(int fd, void *buffer, size_t n) {
    int number_read;
    int total_read;
    char *internal_buffer;
    char read_char;
    
    if(n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    internal_buffer = buffer;
    memset(internal_buffer, 0, BUFFER_SIZE);
    
    total_read = 0;
    while(true) {
        number_read = read(fd, &read_char, 1);
        
        if(number_read == -1) {
            if(errno == EINTR) {
                continue;
            } else {
                return -1;
            }
        } else if(number_read == 0) {
            if(total_read == 0) {
                return 0;
            } else {
                break;
            }
        } else {
            if(total_read < n - 1) {
                total_read++;
                *internal_buffer++ = read_char;
            }
            
            if(read_char == '\n') {
                break;
            }
        }
    }
    
    *internal_buffer = '\0';
    return total_read;
}

static void *receive_thread(void *thread) {
    int client_fd = ((thread_data *) thread)->socket;
    
    while(true) {
        message incoming_message;
        int bytes_handled;
        char buffer[BUFFER_SIZE];
        time_t current_time;
        size_t time_buffer_bytes;
        struct tm *local_time;
        
        incoming_message.header_size = 0;
        incoming_message.headers =
                (char **) malloc(MAX_HEADERS * sizeof(char *));
        incoming_message.message_size = 0;
        incoming_message.message =
                (char **) malloc(MAX_MESSAGES * sizeof(char *));
        
        while(true) {
            bytes_handled = read_line(client_fd, buffer, BUFFER_SIZE);
            
            if(bytes_handled < 1 || strcmp("\r\n", buffer) == 0 ||
                    incoming_message.header_size == MAX_HEADERS) {
                break;
            }
            
            incoming_message.headers[incoming_message.header_size] =
                    (char *) malloc(strlen(buffer));
            strcpy(incoming_message.headers[incoming_message.header_size],
                   buffer);
            incoming_message.header_size++;
        }
    
        current_time = time(NULL);
        local_time = localtime(&current_time);
        if(local_time != NULL) {
            time_buffer_bytes = strftime(buffer, BUFFER_SIZE,
                    "%a, %d %b %Y %T %Z", local_time);
            if(time_buffer_bytes != 0) {
                incoming_message.headers[incoming_message.header_size] =
                        (char *) malloc(strlen("Received-at: ") +
                        strlen(buffer) + strlen("\r\r"));
                strcpy(incoming_message.headers[incoming_message.header_size],
                        "Received-at: ");
                strcat(incoming_message.headers[incoming_message.header_size],
                        buffer);
                strcat(incoming_message.headers[incoming_message.header_size],
                        "\r\n");
                incoming_message.header_size++;
            }
        }
        
        while(true) {
            bytes_handled = read_line(client_fd, buffer, BUFFER_SIZE);
            
            if(bytes_handled < 1 || strcmp("\r\n", buffer) == 0 ||
                incoming_message.message_size == MAX_MESSAGES) {
                break;
            }
            
            incoming_message.message[incoming_message.message_size] =
                    (char *) malloc(strlen(buffer));
            strcpy(incoming_message.message[incoming_message.message_size],
                    buffer);
            
            incoming_message.message_size++;
        }
        
        int x;
        for(x = 0; x < incoming_message.header_size; x++) {
            syslog(LOG_DEBUG, "Header: %s", incoming_message.headers[x]);
        }
        for(x = 0; x < incoming_message.message_size; x++) {
            syslog(LOG_DEBUG, "Message: %s", incoming_message.message[x]);
        }
        
        for(x = incoming_message.header_size-1; x >= 0; x--) {
            free(incoming_message.headers[x]);
        }
        for(x = incoming_message.message_size-1; x >= 0; x--) {
            free(incoming_message.message[x]);
        }
        
        free(incoming_message.headers);
        free(incoming_message.message);
    }
    
    return NULL;
}

void *socket_thread(void *thread) {
    int server_fd = ((thread_data *) thread)->socket;
    struct sockaddr_storage client_addr;
    int client_addr_size;
    int client_fd;
    thread_data this_thread;
    thread_id send_thread_id, receive_thread_id;
    int bytes_handled;
    char buffer[BUFFER_SIZE];
    
    while(true) {
    
        client_addr_size = sizeof(client_addr);
        lock(&accept_lock);
        client_fd = accept(server_fd, (struct sockaddr *) &client_addr,
                &client_addr_size);
        if(client_fd == -1) {
            close(client_fd);
            continue;
        }
        unlock(&accept_lock);
        
        memcpy(&this_thread, thread, sizeof(thread));
        this_thread.socket = client_fd;
        
        start_joinable_thread(&receive_thread_id, receive_thread, &this_thread);
        
        
        
        join_thread(&receive_thread_id);
        
        close(client_fd);
        
    }
    
    return NULL;
}