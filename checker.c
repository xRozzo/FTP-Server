/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"
void check_args(int argc, char **argv)
{
    if (argc != 3 && argc != 2) {
        perror("Invalid usage, use --help flag for more information.\n");
        exit(84);
    }
    if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            printf("Usage: %s port path\n", argv[0]);
            printf("port is the port number on which the "
                "server socket listens\n");
            printf("path is the path to the home directory for the "
                "\033[3mAnonymous\033[0m "
                "user\n");
            exit(0);
        } else
            exit(84);
    }
}

void trim_end(char *str)
{
    char *end = str + strlen(str) - 1;

    while (end > str && (*end == '\n' || *end == '\r' || *end == ' ')) {
        *end = '\0';
        end--;
    }
}

void fork_error(int client_socket, int data_socket)
{
    write(client_socket, "450 Requested file action not taken\r\n",
        sizeof("450 Requested file action not taken\r\n"));
    close(client_socket);
    close(data_socket);
    return;
}
