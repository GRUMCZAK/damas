#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>

static void send_zerosized_reply(int sock, const char *text)
{
    char out[1024];
    memset(out, 0, sizeof(out));
    strncpy(out, text, sizeof(out) - 1);

    if (write(sock, out, sizeof(out)) < 0) {
        fprintf(stderr, "Write error: %s\n", strerror(errno));
    }
}

int main(void)
{
    // =====================
    // UDP MULTICAST SERVER
    // =====================

    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("socket UDP");
        return 1;
    }

    int reuse = 1;
    if (setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt SO_REUSEADDR");
        close(udp_sock);
        return 1;
    }

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_port = htons(1831);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(udp_sock, (struct sockaddr*)&local, sizeof(local)) < 0) {
        perror("bind UDP");
        close(udp_sock);
        return 1;
    }

    struct ip_mreq group;
    inet_pton(AF_INET, "239.0.0.1", &group.imr_multiaddr);
    group.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(udp_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)) < 0) {
        perror("setsockopt IP_ADD_MEMBERSHIP");
        close(udp_sock);
        return 1;
    }

    printf("UDP multicast listening on 239.0.0.1:1831\n");
    fflush(stdout);

    char buf[1024];
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);

    int n = recvfrom(
        udp_sock,
        buf,
        sizeof(buf) - 1,
        0,
        (struct sockaddr*)&sender,
        &sender_len
    );

    if (n < 0) {
        perror("recvfrom");
        close(udp_sock);
        return 1;
    }

    buf[n] = '\0';
    printf("Received UDP request: %s\n", buf);

    struct sockaddr_in target = {0};
    target.sin_family = AF_INET;
    target.sin_port = htons(1830);
    inet_pton(AF_INET, "127.0.0.1", &target.sin_addr);

    const char *server_name = "serwer1";
    if (sendto(
            udp_sock,
            server_name,
            strlen(server_name) + 1,
            0,
            (struct sockaddr*)&target,
            sizeof(target)
        ) < 0)
    {
        perror("sendto");
        close(udp_sock);
        return 1;
    }

    printf("Sent UDP response to 127.0.0.1:1830\n");
    close(udp_sock);

    // =====================
    // TCP SERVER
    // =====================

    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0) {
        perror("socket TCP");
        return 1;
    }

    reuse = 1;
    if (setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt SO_REUSEADDR TCP");
        close(tcp_sock);
        return 1;
    }

    struct sockaddr_in tcp_addr = {0};
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_port = htons(1864);
    inet_pton(AF_INET, "192.168.0.30", &tcp_addr.sin_addr);

    if (bind(tcp_sock, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr)) < 0) {
        perror("bind TCP");
        close(tcp_sock);
        return 1;
    }

    if (listen(tcp_sock, 5) < 0) {
        perror("listen");
        close(tcp_sock);
        return 1;
    }

    printf("TCP listening on 192.168.0.30:1864\n");
    fflush(stdout);

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int client_sock = accept(tcp_sock, (struct sockaddr*)&client, &client_len);
    if (client_sock < 0) {
        perror("accept");
        close(tcp_sock);
        return 1;
    }

    printf("Client connected\n");

    char tcp_buf[1024];
    memset(tcp_buf, 0, sizeof(tcp_buf));

    int r = recv(client_sock, tcp_buf, sizeof(tcp_buf) - 1, 0);
    if (r < 0) {
        perror("recv login");
        close(client_sock);
        close(tcp_sock);
        return 1;
    }

    tcp_buf[r] = '\0';
    printf("TCP received login: %s\n", tcp_buf);

    send_zerosized_reply(client_sock, "ok\n1 jesli costam\n2 jesli costam\n");

    while (1) {
        memset(tcp_buf, 0, sizeof(tcp_buf));

        r = read(client_sock, tcp_buf, sizeof(tcp_buf) - 1);
        if (r < 0) {
            perror("read");
            break;
        }
        if (r == 0) {
            printf("Client closed connection\n");
            break;
        }

        tcp_buf[r] = '\0';
        printf("Odebrano ruch klienta: %s\n", tcp_buf);

        char reply[1024];
        memset(reply, 0, sizeof(reply));

        printf("Nadaj odpowiedz: ");
        fflush(stdout);

        if (fgets(reply, sizeof(reply), stdin) == NULL) {
            strcpy(reply, "brak odpowiedzi\n");
        }

        if (write(client_sock, reply, sizeof(reply)) < 0) {
            perror("write");
            break;
        }
    }

    close(client_sock);
    close(tcp_sock);
    return 0;
}