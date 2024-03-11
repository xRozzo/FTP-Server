/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"

bool check_input_buffer(char **buffer, int client_socket)
{
    char *reply;

    if (buffer[1] == NULL) {
        reply = "550 Requested action not taken\r\n";
        write(client_socket, reply, strlen(reply));
        return false;
    }
    return true;
}

FILE *open_file(char *file_path, int client_socket)
{
    FILE *file;
    char *reply;

    file = fopen(file_path, "rb");
    if (!file) {
        reply = "450 Requested file action not taken\r\n";
        write(client_socket, reply, strlen(reply));
    }
    return file;
}

long get_file_size(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0) {
        return st.st_size;
    } else {
        perror("Could not determine file size");
        return -1;
    }
}

void check_size(int file_size, int client_socket)
{
    if (file_size == -1) {
        perror("error while getting size");
        close(client_socket);
        exit(84);
    }
}
