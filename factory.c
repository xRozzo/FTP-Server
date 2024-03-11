/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"

int bind_and_listen(int data_sacket, socklen_t sock_addr_len,
    struct sockaddr_in data_socket_addr)
{
    if (bind(data_sacket, (struct sockaddr *) &data_socket_addr,
        sock_addr_len) == -1) {
        perror("bind");
        close(data_sacket);
        return -1;
    }
    if (listen(data_sacket, SOMAXCONN) == -1) {
        perror("listen");
        close(data_sacket);
        return -1;
    }
    return 0;
}

int socket_creation(int port, Server *server)
{
    struct sockaddr_in sock_addr;
    socklen_t address_len = sizeof(sock_addr);
    int server_socket = 0;

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        perror("socket");
    if (bind_and_listen(server_socket, address_len, sock_addr) == -1)
        return -1;
    if (getsockname(server_socket, (struct sockaddr *) &sock_addr,
                    &address_len) == -1)
        return -1;
    server->address = sock_addr;
    return server_socket;
}

int create_active_ds(int port, char *server_ip)
{
    struct sockaddr_in data_socket_addr;
    socklen_t sock_addr_len;
    int data_sacket = socket(AF_INET, SOCK_STREAM, 0);

    if (data_sacket == -1) {
        perror("socket");
        return -1;
    }
    memset(&data_socket_addr, 0, sizeof(data_socket_addr));
    data_socket_addr.sin_family = AF_INET;
    data_socket_addr.sin_addr.s_addr = INADDR_ANY;
    data_socket_addr.sin_port = htons(port);
    sock_addr_len = sizeof(data_socket_addr);
    if (bind_and_listen(data_sacket, sock_addr_len, data_socket_addr) == -1)
        return -1;
    return data_sacket;
}

int create_data_socket(int *port)
{
    int lint_res;
    struct sockaddr_in data_sckaddr;
    int data_socket = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t sc_addr_len = sizeof(data_sckaddr);

    memset(&data_sckaddr, 0, sizeof(data_sckaddr));
    data_sckaddr.sin_family = AF_INET;
    data_sckaddr.sin_addr.s_addr = INADDR_ANY;
    data_sckaddr.sin_port = htons(0);
    sc_addr_len = sizeof(data_sckaddr);
    if (bind_and_listen(data_socket, sc_addr_len, data_sckaddr) == -1)
        return -1;
    if (getsockname(data_socket, (struct sockaddr *) &data_sckaddr,
                    &sc_addr_len) == -1) {
        perror("getsockname");
        close(data_socket);
        return -1;
    }
    *port = ntohs(data_sckaddr.sin_port);
    return data_socket;
}
