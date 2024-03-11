/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"

void user_command(int client_socket, char **buffer, UNUSED Server *server)
{
    char *reply;

    if (buffer[1] == NULL || (strcmp(buffer[1], "Anonymous") == 0)) {
        if (buffer[1] == NULL) {
            reply = "530 not username porvided\r\n";
        } else {
            reply = "331 User name okay, need password.\r\n";
        }
        server->clients[get_c(client_socket, server)].is_username_correct =
            true;
        write(client_socket, reply, strlen(reply));
    } else {
        reply = "530\r\n";
        write(client_socket, reply, strlen(reply));
    }
}

void pass_command(int client_socket, char **buffer, Server *server)
{
    char *reply;

    if (buffer[1] == NULL) {
        reply = "530 invalid password\r\n";
        write(client_socket, reply, strlen(reply));
    } else {
        reply = "230 User logged in, proceed.\r\n";
        server->clients[get_c(client_socket, server)].is_auth = true;
        write(client_socket, reply, strlen(reply));
    }
}
