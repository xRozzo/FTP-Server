/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"

bool check_dir_and_buffer(DIR **dir, char **reply, int client_socket,
    char **buffer)
{
    if (buffer[1] == NULL) {
        *reply = "550 Requested action not taken\r\n";
        write(client_socket, *reply, strlen(*reply));
        return false;
    }
    *dir = opendir(buffer[1]);
    if (*dir == NULL) {
        perror("opendir");
        *reply = "550 Requested action not taken\r\n";
        write(client_socket, *reply, strlen(*reply));
        return false;
    } else {
        closedir(*dir);
    }
    return true;
}

bool fill_path(char **buffer, char *resolved_path, char **reply,
    int client_socket)
{
    if (realpath(buffer[1], resolved_path) == NULL) {
        perror("realpath failed");
        *reply = "550 Requested action not taken\r\n";
        write(client_socket, *reply, strlen(*reply));
        return false;
    }
    return true;
}
