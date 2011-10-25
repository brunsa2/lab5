/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Network startup thread
 */

#include "network.h"

extern login_info login;
int network_error_code;
int sock_fd;

void *network_thread(void *argument) {
    thread_id send_thread_id, receive_thread_id;
    struct addrinfo hints, *servinfo, *current_addr;
    int gai_error_code;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if((gai_error_code = getaddrinfo(login.server, login.port, &hints, &servinfo)) != 0) {
        network_error_code = -1;
        return NULL;
    }
    
    for(current_addr = servinfo; current_addr != NULL; current_addr =       
            current_addr->ai_next) {
        if((sock_fd = socket(current_addr->ai_family,
                              current_addr->ai_socktype, current_addr->ai_protocol)) == -1) {
            continue;
        }
        
        if(connect(sock_fd, current_addr->ai_addr, current_addr->ai_addrlen) == -1) {
            close(sock_fd);
            continue;
        }
        
        break;
    }
    
    if(current_addr == NULL) {
        network_error_code = -1;
        return NULL;
    }
    
    freeaddrinfo(servinfo);
    
    network_ready();
    
    write(sock_fd,
          "Join\r\rUsername: brunsa2\r\n\r\n\r\n",
          strlen("Join\r\rUsername: brunsa2\r\n\r\n\r\n"));
    write(sock_fd,
          "Message\r\nUsername: brunsa2\r\n\r\nHi! How are you?\r\n\r\n",
          strlen("Message\r\nUsername: brunsa2\r\n\r\nHi! How are you?\r\n\r\n"));
    
    //start_joinable_thread(&send_thread_id, send_thread, &sock_fd);
    start_joinable_thread(&receive_thread_id, receive_thread, &sock_fd);
    
    join_thread(&send_thread_id);
    join_thread(&receive_thread_id);
    
    close(sock_fd);
    
    return NULL;
}