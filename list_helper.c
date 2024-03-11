/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"

bool check_pipe(FILE *pipe, int client_socket)
{
    char *error_msg;

    if (!pipe) {
        error_msg = "425\r\n";
        write(client_socket, error_msg, strlen(error_msg));
        return false;
    }
    return true;
}
