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
        bool should_leave = false;
        message incoming_message;
        int bytes_handled;
        char buffer[BUFFER_SIZE];
        char username_buffer[BUFFER_SIZE], username[BUFFER_SIZE], *username_position;
        time_t current_time;
        size_t time_buffer_bytes;
        struct tm *local_time;
        int do_not_dispatch_incoming_message = 0, line_index;
        
        incoming_message.header_size = 0;
        incoming_message.headers =
                (char **) malloc(MAX_HEADERS * sizeof(char *));
        incoming_message.message_size = 0;
        incoming_message.message =
                (char **) malloc(MAX_MESSAGES * sizeof(char *));
        
        while(true) {
            memset(buffer, 0, BUFFER_SIZE);
            bytes_handled = read_line(client_fd, buffer, BUFFER_SIZE);
            
            if(bytes_handled < 1 || strcmp("\r\n", buffer) == 0 ||
                    incoming_message.header_size == MAX_HEADERS) {
                syslog(LOG_INFO, "Received header %s", buffer);
                //should_leave == bytes_handled < 1;
                break;
            }
            
            syslog(LOG_INFO, "Received header %s", buffer);
            
            strncpy(username_buffer, buffer, strlen("Username: "));
            if(strcmp(username_buffer, "Username: ") == 0) {
                username_position = buffer + strlen("Username: ");
                strncpy(username, username_position, strlen(username_position) - 2);
                
            }
                        
            incoming_message.headers[incoming_message.header_size] =
                    (char *) malloc(strlen(buffer));
            strcpy(incoming_message.headers[incoming_message.header_size],
                   buffer);
            incoming_message.header_size++;
        }
        
        while(true) {
            bytes_handled = read_line(client_fd, buffer, BUFFER_SIZE);
            
            if(bytes_handled < 1 || strcmp("\r\n", buffer) == 0 ||
                    incoming_message.message_size == MAX_MESSAGES) {
                syslog(LOG_INFO, "Received message %s", buffer);
                //should_leave = bytes_handled < 1;
                break;
            }
            
            syslog(LOG_INFO, "Received message %s", buffer);
            
            incoming_message.message[incoming_message.message_size] =
                    (char *) malloc(strlen(buffer));
            strcpy(incoming_message.message[incoming_message.message_size],
                    buffer);
            
            incoming_message.message_size++;
        }
        
        current_time = time(NULL);
        local_time = localtime(&current_time);
        if(local_time != NULL && !should_leave) {
            time_buffer_bytes = strftime(buffer, BUFFER_SIZE,
                                         "%a, %d %b %Y %T %Z", local_time);
            if(time_buffer_bytes != 0) {
                incoming_message.headers[incoming_message.header_size] =
                (char *) malloc(strlen("Received-at: ") +
                                strlen(buffer) + strlen("\r\r") + 1);
                strcpy(incoming_message.headers[incoming_message.header_size],
                       "Received-at: ");
                strcat(incoming_message.headers[incoming_message.header_size],
                       buffer);
                strcat(incoming_message.headers[incoming_message.header_size],
                       "\r\n");
                incoming_message.header_size++;
            }
        }
        
        if(incoming_message.header_size > 0 && strcmp(incoming_message.headers[0], "Join\r\n") == 0) {
            message *join_message = malloc(sizeof(message));
            join_message->header_size = 1;
            join_message->message_size = 1;
            join_message->headers = (char **) malloc(MAX_HEADERS * sizeof(char *));
            join_message->message = (char **) malloc(MAX_MESSAGES * sizeof(char *));
            join_message->headers[0] = (char *) malloc(strlen("Message\r\n") + 1);
            strcpy(join_message->headers[0], "Message\r\n");
            join_message->message[0] = (char *) malloc(BUFFER_SIZE);
            strcpy(join_message->message[0], username);
            strcat(join_message->message[0], " has joined the chat\r\n");
            
            if(local_time != NULL && !should_leave) {
                time_buffer_bytes = strftime(buffer, BUFFER_SIZE,
                                             "%a, %d %b %Y %T %Z", local_time);
                if(time_buffer_bytes != 0) {
                    join_message->headers[join_message->header_size] =
                    (char *) malloc(strlen("Received-at: ") +
                                    strlen(buffer) + strlen("\r\r") + 1);
                    strcpy(join_message->headers[join_message->header_size],
                           "Received-at: ");
                    strcat(join_message->headers[join_message->header_size],
                           buffer);
                    strcat(join_message->headers[join_message->header_size],
                           "\r\n");
                    join_message->header_size++;
                }
            }
            
            dispatch_message(join_message);
            do_not_dispatch_incoming_message = 1;
            
            for(line_index = 0; line_index < join_message->header_size; line_index++) {
                free(join_message->headers[line_index]);
            }
            for(line_index = 0; line_index < join_message->message_size; line_index++) {
                free(join_message->message[line_index]);
            }
            free(join_message->headers);
            free(join_message->message);
            free(join_message);
        }
        
        if(incoming_message.header_size > 0 && strcmp(incoming_message.headers[0], "Leave\r\n") == 0) {
            message *leave_message = malloc(sizeof(message));
            leave_message->header_size = 1;
            leave_message->message_size = 1;
            leave_message->headers = (char **) malloc(MAX_HEADERS * sizeof(char *));
            leave_message->message = (char **) malloc(MAX_MESSAGES * sizeof(char *));
            leave_message->headers[0] = (char *) malloc(strlen("Message\r\n") + 1);
            strcpy(leave_message->headers[0], "Message\r\n");
            leave_message->message[0] = (char *) malloc(BUFFER_SIZE);
            strcpy(leave_message->message[0], username);
            strcat(leave_message->message[0], " has left the chat\r\n");
            
            if(local_time != NULL && !should_leave) {
                time_buffer_bytes = strftime(buffer, BUFFER_SIZE,
                                             "%a, %d %b %Y %T %Z", local_time);
                if(time_buffer_bytes != 0) {
                    leave_message->headers[leave_message->header_size] =
                    (char *) malloc(strlen("Received-at: ") +
                                    strlen(buffer) + strlen("\r\r") + 1);
                    strcpy(leave_message->headers[leave_message->header_size],
                           "Received-at: ");
                    strcat(leave_message->headers[leave_message->header_size],
                           buffer);
                    strcat(leave_message->headers[leave_message->header_size],
                           "\r\n");
                    leave_message->header_size++;
                }
            }
            
            dispatch_message(leave_message);
            do_not_dispatch_incoming_message = 1;
            
            for(line_index = 0; line_index < leave_message->header_size; line_index++) {
                free(leave_message->headers[line_index]);
            }
            for(line_index = 0; line_index < leave_message->message_size; line_index++) {
                free(leave_message->message[line_index]);
            }
            free(leave_message->headers);
            free(leave_message->message);
            free(leave_message);
        }
        
        if(incoming_message.header_size > 0 && strcmp(incoming_message.headers[0], "Leave\r\n") == 0) {
            ((thread_data *) thread)->should_shutdown = 1;
        }
        
        if(!do_not_dispatch_incoming_message) {
            dispatch_message(&incoming_message);
        }
        do_not_dispatch_incoming_message = 0;
        
        for(line_index = 0; line_index < incoming_message.header_size; line_index++) {
            free(incoming_message.headers[line_index]);
        }
        for(line_index = 0; line_index < incoming_message.message_size; line_index++) {
            free(incoming_message.message[line_index]);
        }
        free(incoming_message.headers);
        free(incoming_message.message);
        
        if(should_leave) {
            break;
        }
        
        if(((thread_data *) thread)->should_shutdown == 1) {
            return NULL;
        }
    }
    
    return NULL;
}

void *socket_thread(void *thread) {
    int server_fd = ((thread_data *) thread)->socket;
    struct sockaddr_storage client_addr;
    int client_addr_size;
    int client_fd;
    thread_id send_thread_id, receive_thread_id;
    int bytes_handled;
    char buffer[BUFFER_SIZE];
    time_t current_time;
    size_t time_buffer_bytes;
    struct tm *local_time;
    
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

        ((thread_data *) thread)->socket = client_fd;
        
        start_joinable_thread(&receive_thread_id, receive_thread, thread);
        
        ll_clear(((thread_data *) thread)->message_queue);
        
        for(;;) {
            sleep(1);
            if(ll_size(((thread_data *) thread)->message_queue)) {
                message *sending_message = ll_get(((thread_data *) thread)->message_queue, 0);
                
                current_time = time(NULL);
                local_time = localtime(&current_time);
                if(local_time != NULL) {
                    time_buffer_bytes = strftime(buffer, BUFFER_SIZE,
                                                 "%a, %d %b %Y %T %Z", local_time);
                    if(time_buffer_bytes != 0) {
                        sending_message->headers[sending_message->header_size] =
                        (char *) malloc(strlen("Echoed-at: ") +
                                        strlen(buffer) + strlen("\r\r"));
                        strcpy(sending_message->headers[sending_message->header_size],
                               "Echoed-at: ");
                        strcat(sending_message->headers[sending_message->header_size],
                               buffer);
                        strcat(sending_message->headers[sending_message->header_size],
                               "\r\n");
                        sending_message->header_size++;
                    }
                }
                
                int line_index;
                for(line_index = 0; line_index < sending_message->header_size; line_index++) {
                    if(write(client_fd, sending_message->headers[line_index], strlen(sending_message->headers[line_index])) != strlen(sending_message->headers[line_index])) {
                        syslog(LOG_INFO, "Error writing");
                    }
                    free(sending_message->headers[line_index]);
                }
                write(client_fd, "\r\n", strlen("\r\n"));
                syslog(LOG_INFO, "Message");
                for(line_index = 0; line_index < sending_message->message_size; line_index++) {
                    if(write(client_fd, sending_message->message[line_index], strlen(sending_message->message[line_index])) != strlen(sending_message->message[line_index])) {
                        syslog(LOG_INFO, "Error writing");
                    }
                    free(sending_message->message[line_index]);
                }
                write(client_fd, "\r\n", strlen("\r\n"));
                free(sending_message->headers);
                free(sending_message->message);
                free(sending_message);
                ll_remove(((thread_data *) thread)->message_queue, 0);
            }
            
            if(((thread_data *) thread)->should_shutdown == 1) {
                break;
            }
        }
        
        ll_clear(((thread_data *) thread)->message_queue);
        
        join_thread(&receive_thread_id);
        
        close(client_fd);
        
    }
    
    return NULL;
}