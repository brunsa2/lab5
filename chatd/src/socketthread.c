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
        
        while(true) {
            bytes_handled = read_line(client_fd, buffer, BUFFER_SIZE);
            if(bytes_handled < 1) {
                break;
            }
            
            if(strcmp("\r\n", buffer) == 0) {
                break;
            }
            
            if(incoming_message.header_size == MAX_HEADER_SIZE) {
                break;
            }
            
            incoming_message.headers[incoming_message.header_size] =
                    (char *) malloc(strlen(buffer));
            strcmp(incoming_message.headers[incoming_message.header_size],
                    buffer);
            incoming_message.header_size++;
        }
        
        int x;
        for(x = 0; x < incoming_message.header_size; x++) {
            syslog(LOG_DEBUG, "Line: %s", incoming_message.headers[x]);
        }
        
        for(x = incoming_message.header_size-1; x >= 0; x--) {
            free(incoming_message.headers[x]);
        }
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
        
        /*while(true) {
            bytes_handled = read(client_fd, buffer, BUFFER_SIZE);
            if(bytes_handled < 1) {
                syslog(LOG_CRIT, "Could not read data");
                if(errno == EPIPE) {
                    syslog(LOG_CRIT, "Broken pipe");
                }
                break;
            }
            buffer[bytes_handled] = '\0';
            
            if((bytes_handled = strcmp(buffer, "exit\r\n")) == 0) {
                syslog(LOG_INFO, "Client wants out");
                break;
            } else {
                syslog(LOG_INFO, "Client continues to talk... %d", bytes_handled);
            }
            
            bytes_handled = write(client_fd, buffer, strlen(buffer));
            if(bytes_handled < 1) {
                syslog(LOG_CRIT, "Could not write data");
                if(errno == EPIPE) {
                    syslog(LOG_CRIT, "Broken pipe");
                }
                break;
            }
        }*/
        
        
        join_thread(&receive_thread_id);
        
        
        
        close(client_fd);
        
    }
    
    return NULL;
}