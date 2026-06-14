// send a multicast game request - UDP
// receive server response
// connect to server via dns - TCP
// send a login 
// receive a response
// send a password
// start a game or view scoreboard
// in game - send move, receive game status
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

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

    char msg[] = "CLIENT:192.168.0.62";
    sendto(multisockfd, msg, sizeof(msg), 0, (struct sockaddr*)&multiaddr, sizeof(multiaddr));
    close(multisockfd);
    
    socklen_t len = sizeof(servaddr);
    if( recvfrom(sockfd, servname, sizeof(servname), 0, (struct sockaddr*)&servaddr, &len) < 0)
    {
        printf("Recvfrom error");
    }
    printf("Server name is: %s\n", servname);

    close(sockfd);
}

int main()
{
    /*int sockfd = socket(AF_INET, SOCK_PACKET, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htonl(1863);

    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    listen()*/
    char servname[100];
    printf("Witaj! Podaj nazwę serwera, lub kliknij x, aby go wyszukać.");
    fgets(servname, sizeof(servname), stdin);
    if(strcmp(servname, "x") != 0)
        send_game_req();

    
}