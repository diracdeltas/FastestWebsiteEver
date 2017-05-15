/** @file main.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#ifdef DEFLATE
#include <zlib.h>
#define HTTP_HEADER_SIZE 64 // for payloads up to 9999
#endif

#define PORT "80"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    FILE *fp;
    char    *buffer;
    char    *send_buffer;
    long    numbytes;
    long    hdrbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt reuseaddr");
            exit(1);
        }
#ifdef SO_BUSY_POLL
        if (setsockopt(sockfd, SOL_SOCKET, SO_BUSY_POLL, &yes, sizeof(int)) == -1) {
            perror("setsockopt busypoll");
            exit(1);
        }
#endif
        if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int)) == -1) {
            perror("setsockopt tcp nodelay");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    fp = fopen("index.html","r");
 
    if(fp == NULL) {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_END);
    numbytes = ftell(fp);
    fseek(fp, 0L, SEEK_SET);	
     
    buffer = (char*)calloc(numbytes, sizeof(char));	
    if(buffer == NULL) {
        return 1;
    }
     
    fread(buffer, sizeof(char), numbytes, fp);
    fclose(fp);

#ifdef DEFLATE
    z_stream defstream;
    char     *obuf;

    // Compress file
    //-----------------------------
    if ((obuf = malloc (numbytes * sizeof (char))) == NULL) return 1;
      
    defstream.zalloc = Z_NULL; 
    defstream.zfree = Z_NULL;

    // Input
    defstream.avail_in = numbytes; 
    defstream.next_in = (Bytef *) buffer;

    // Output
    defstream.avail_out = numbytes;
    defstream.next_out = (Bytef *)obuf; 
    
    // Match default Python script configuration
    deflateInit2 (&defstream, Z_BEST_COMPRESSION, Z_DEFLATED, 
		  -15, 8, Z_DEFAULT_STRATEGY);
    deflate (&defstream, Z_FINISH);
    deflateEnd (&defstream);

    numbytes = defstream.total_out;
    free (buffer);
    //-----------------------------
#endif

#ifdef DEFLATE     
    send_buffer = (char*)calloc(numbytes + HTTP_HEADER_SIZE, sizeof(char));	
#else
    send_buffer = (char*)calloc(numbytes, sizeof(char));	
#endif
    hdrbytes = sprintf(send_buffer, "HTTP/1.1 200 k\nContent-Length: %ld\ncontent-encoding: deflate\n\n", numbytes);
#ifdef DEFLATE
    memcpy(send_buffer+hdrbytes, obuf, numbytes);
    free (obuf);
#else
    memcpy(send_buffer+hdrbytes, buffer, numbytes);
#endif

    printf ("INFO: TCP payload size: %ld\n", numbytes + hdrbytes);
    printf("server: waiting for connections on port %s...\n", PORT);

    while(1) {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        if (send(new_fd, send_buffer, numbytes+hdrbytes, 0) == -1) {
            perror("send");
        }
        close(new_fd);
    }

    return 0;
}
