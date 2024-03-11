/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"

void cwd_command(int client_socket, char **buffer, Server *server)
{
    DIR *dir;
    char *reply = NULL;
    char *temp_cwd;
    char resolved_path[1024];
    int res = 0;

    if (!check_dir_and_buffer(&dir, &reply, client_socket, buffer))
        return;
    if (!fill_path(buffer, resolved_path, &reply, client_socket))
        return;
    if (server->clients[get_c(client_socket, server)].cwd != NULL) {
        free(server->clients[get_c(client_socket, server)].cwd);
        server->clients[get_c(client_socket, server)].cwd = NULL;
    }
    server->clients[get_c(client_socket, server)].cwd =
        (char *) malloc(strlen(resolved_path) + 1);
    strcpy(server->clients[get_c(client_socket, server)].cwd, resolved_path);
    reply = "250\r\n";
    write(client_socket, reply, strlen(reply));
}

void pwd_command(int client_socket, char **buffer, Server *server)
{
    char reply[1024];

    sprintf(reply, "257 cwd = %s\r\n",
            server->clients[get_c(client_socket, server)].cwd);
    write(client_socket, reply, strlen(reply));
}

void add_final_char(int i, Server *server, int client_socket)
{
    if (i > 0) {
            server->clients[get_c(client_socket, server)].cwd[i] = '\0';
        } else if (i == 0) {
            server->clients[get_c(client_socket, server)].cwd[i + 1] = '\0';
        }
}

void cdup_command(int client_socket, char **buffer, Server *server)
{
    char *reply = NULL;
    int i = 0;

    if (!check_len(server, client_socket))
        return;
    if (strlen(server->clients[get_c(client_socket, server)].cwd) > 1) {
        i = strlen(server->clients[get_c(client_socket, server)].cwd) - 1;
        while (i >= 0 &&
            server->clients[get_c(client_socket,
                server)].cwd[i] != '/') {
            i--;
        }
        add_final_char(i, server, client_socket);
    }
    reply = "200 Directory changed to parent directory.\r\n";
    write(client_socket, reply, strlen(reply));
}

void dele_command(int client_socket, char **buffer, Server *server)
{
    char *reply = NULL;
    int rem = 0;

    if (buffer[1] == NULL) {
        reply = "550 Requested action not taken\r\n";
    } else {
        rem = remove(buffer[1]);
        if (rem != 0)
            reply = "550 Requested action not taken\r\n";
        else
            reply = "250\r\n";
    }
    write(client_socket, reply, strlen(reply));
}
