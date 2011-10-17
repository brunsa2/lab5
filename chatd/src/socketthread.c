/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 15 October 2011
 *
 * Main thread for each socket
 */

#include "socketthread.h"

static void sigchld_handler(int s) {
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

void *get_in_addr(struct sockaddr *sa) {
    if(sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void server(void) {
    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((rv = getaddrinfo(NULL, "65500", &hints, &servinfo)) != 0) {
        syslog(LOG_CRIT, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }
    
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            syslog(LOG_INFO, "server: socket");
            continue;
        }
        
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            syslog(LOG_INFO, "setsockopt");
            exit(EXIT_FAILURE);
        }
        
        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            syslog(LOG_INFO, "server: bind");
            continue;
        }
        
        break;
    }
    
    if(p == NULL) {
        syslog(LOG_CRIT, "server: failed to bind");
        exit(EXIT_FAILURE);
    }
    
    freeaddrinfo(servinfo);
    
    if(listen(sockfd, 10) == -1) {
        syslog(LOG_CRIT, "listen");
        exit(EXIT_FAILURE);
    }
    
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1) {
        syslog(LOG_CRIT, "sigaction");
        exit(EXIT_FAILURE);
    }
    
    syslog(LOG_INFO, "server: waiting for connections...");
    
    while(1) {
        sin_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if(new_fd == -1) {
            syslog(LOG_CRIT, "accept");
            continue;
        }
        
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)
                &their_addr), s, sizeof(s));
        syslog(LOG_INFO, "server: got connection from %s", s);
        
        if(!fork()) {
            close(sockfd);
            if(send(new_fd, "Hello, World!", 13, 0) == -1) {
                syslog(LOG_CRIT, "send");
            }
            close(new_fd);
            exit(EXIT_SUCCESS);
        }
        close(new_fd);
    }
}