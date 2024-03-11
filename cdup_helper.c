/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"

bool check_len(Server *server, int client_socket)
{
    if (strlen(server->clients[get_c(client_socket, server)].cwd) ==
        strlen(server->cwd)) {
        write(client_socket, "250\r\n",
            strlen("250\r\n"));
        return false;
    }
    return true;
}
