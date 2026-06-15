// send a multicast game request - UDP
// receive server response
// connect to server via dns - TCP
// send a login/password 
// receive a response
// start a game or view scoreboard
// in game - send move, receive game status
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>


// client multicast port - 1830
// server multicast port - 1831
// client unicast port - 1863
// server unicast port - 1864


char servname[1024];

void send_game_req()
{
    int multisockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in multiaddr, servaddr;

    multiaddr.sin_family = AF_INET;
    multiaddr.sin_port = htons(1831);
    inet_pton(AF_INET, "239.0.0.1", &multiaddr.sin_addr);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1830);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr); 
    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    char msg[] = "serwer1";
    sendto(multisockfd, msg, sizeof(msg), 0, (struct sockaddr*)&multiaddr, sizeof(multiaddr));
    close(multisockfd);
    
    socklen_t len = sizeof(servaddr);
    if( recvfrom(sockfd, servname, sizeof(servname), 0, (struct sockaddr*)&servaddr, &len) < 0)
    {
        printf("Recvfrom error");
    }
    printf("Nazwa serwera to: %s\n", servname);

    close(sockfd);
}

int main()
{
    printf("Witaj! Podaj nazwę serwera, lub kliknij x, aby go wyszukać.\n");
    fgets(servname, sizeof(servname), stdin);
    if(strcmp(servname, "x") != 0)
        send_game_req();

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    // 192.168.0.30 defined as serwer1 in /etc/hosts
    //printf("servname: %s\n", servname);
    int rc = getaddrinfo(servname, "1864", &hints, &res);
    if(rc != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return 1;
    }

    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    struct sockaddr_in cli_addr;
    cli_addr.sin_family = res->ai_family;
    inet_pton(res->ai_family, "192.168.0.29", &cli_addr.sin_addr);
    cli_addr.sin_port = htons(1863);

    bind(fd, (struct sockaddr*)&cli_addr, sizeof(cli_addr));
    //printf("res: ");
    if( connect(fd, res->ai_addr, res->ai_addrlen) < 0)
    {
        fprintf(stderr, "Connect error: %s\n", strerror(errno));
        return -1;
    }

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    printf("Podaj login i hasło w formacie login/hasło i naciśnij ENTER\n");
    fgets(buf, sizeof(buf), stdin);

    if( write(fd, buf, strlen(buf)) < 0)
    {
        fprintf(stderr, "Write error: %s\n", strerror(errno));
        return -1;   
    }

    if( read(fd, buf, sizeof(buf)) < 0)
    {
        fprintf(stderr, "Read error: %s\n", strerror(errno));
        return -1;   
    }

    printf("Odpowiedź serwera na próbę logowania: %s\n", buf);

    while(1 == 1)
    {
        printf("Wyślij swoją akcję: ");
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        if( write(fd, buf, strlen(buf)) < 0)
        {
            fprintf(stderr, "Write error: %s\n", strerror(errno));
            return -1;   
        }

        memset(buf, 0, sizeof(buf));
        if( read(fd, buf, sizeof(buf)) < 0)
        {
            fprintf(stderr, "Read error: %s\n", strerror(errno));
            return -1;   
        }
        printf("Odpowiedź serwera: %s\n", buf);
    }
}