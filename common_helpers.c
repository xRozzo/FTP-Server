/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"
void transfer_success(char **reply, int client_socket)
{
    *reply = "226 Transfer complete.\r\n";
    write(client_socket, *reply, strlen(*reply));
}

void transfer_error(char **reply, int client_socket)
{
    perror("Error sending file");
    *reply = "450 Requested file action not taken\r\n";
    write(client_socket, *reply, strlen(*reply));
}

void opening_message(char **reply, int client_socket)
{
    *reply = "150 Opening data connection.\r\n";
    write(client_socket, *reply, strlen(*reply));
}

int get_c(int client_socket, Server *server)
{
    for (int i = 0; server->clients[i].fd; i++) {
        if (server->clients[i].fd == client_socket) {
            return i;
        }
    }
    return -1;
}
