/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"

void help_command(int client_socket, char **buffer, Server *server)
{
    char *reply = NULL;

    reply = "214\r\n";
    write(client_socket, reply, strlen(reply));
}

void noop_command(int client_socket, char **buffer, Server *server)
{
    char *reply = NULL;

    if (buffer[1] != NULL) {
        reply = "500\r\n";
    } else {
        reply = "200\r\n";
    }
    write(client_socket, reply, strlen(reply));
}

void quit_command(int client_socket, char **buffer, Server *server)
{
    char *reply = NULL;

    reply = "221\r\n";
    close(client_socket);
    write(client_socket, reply, strlen(reply));
}

void pasv_command(int client_socket, char **buffer, Server *server)
{
    char reply[128];
    int port = 0;
    char *server_ip = inet_ntoa(server->address.sin_addr);
    int data_socket = create_data_socket(&port);
    char formatted_ip[INET_ADDRSTRLEN];

    check_socket(data_socket, client_socket);
    strcpy(formatted_ip, server_ip);
    for (int i = 0; formatted_ip[i]; i++) {
        if (formatted_ip[i] == '.') {
            formatted_ip[i] = ',';
        }
    }
    server->clients[client_socket].data_socket = data_socket;
    snprintf(reply, sizeof(reply), "227 Entering Passive Mode (%s,%d,%d)\r\n",
        formatted_ip, port / 256, port % 256);
    write(client_socket, reply, strlen(reply));
}

void port_command(int client_socket, char **buffer, Server *server)
{
    char *reply;
    char *ip = NULL;
    int port = 0;
    int data_socket = 0;

    if (!check_buffer(buffer, client_socket))
        return;
    ip = get_ip(buffer[1]);
    port = get_port(buffer[1]);
    data_socket = create_active_ds(port, ip);
    server->clients[client_socket].data_socket = data_socket;
    check_socket(data_socket, client_socket);
    write(client_socket, "200\r\n", strlen("200\r\n"));
}
