/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"

void check_cwd(Server *server, int i)
{
    if (server->clients[i].cwd == NULL) {
        perror("malloc failed for server->clients[i].cwd");
        exit(EXIT_FAILURE);
    }
}

void check_sd(Server *server, int sd)
{
    if (sd > server->max_sd) {
        server->max_sd = sd;
    }
}
