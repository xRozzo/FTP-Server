/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void write_list(int bytes_read, char *read_buffer, FILE *pipe, int data_socket)
{
    while (bytes_read > 0) {
        bytes_read = fread(read_buffer, 1, sizeof(read_buffer), pipe);
        write(data_socket, read_buffer, bytes_read);
    }
}

void list_command(int client_socket, char **buffer, Server *server)
{
    char command[256];
    char read_buffer[1024];
    ssize_t bytes_read;
    FILE *pipe = NULL;
    char *completion_msg;
    int data_socket =
        server->clients[get_c(client_socket, server)].data_socket;

    completion_msg = "150\r\n";
    write(client_socket, completion_msg, strlen(completion_msg));
    snprintf(command, sizeof(command), "ls -l %s",
        server->clients[get_c(client_socket, server)].cwd);
    pipe = popen(command, "r");
    if (!check_pipe(pipe, client_socket))
        return;
    write_list(bytes_read, read_buffer, pipe, data_socket);
    pclose(pipe);
    completion_msg = "226\r\n";
    write(client_socket, completion_msg, strlen(completion_msg));
}

void auxiliar_handler(int client_socket)
{
    write(client_socket, "150\r\n", strlen("150\r\n"));
    write(client_socket, "226\r\n", strlen("226\r\n"));
}

void stor_command(int client_socket, char **buffer, Server *server)
{
    char *file_path;
    int data_socket = server->clients[client_socket].data_socket;
    pid_t pid;

    if (!validate_buffer_and_open_data_socket(buffer, client_socket,
        data_socket)) {
        return;
    }
    file_path = buffer[1];
    pid = fork();
    if (pid == -1) {
        fork_error(client_socket, data_socket);
    } else if (pid == 0) {
        auxiliar_handler(client_socket);
    } else {
        close(data_socket);
    }
}

void retr_command(int client_socket, char **buffer, Server *server)
{
    char *file_path;
    char *reply;
    FILE *file;
    int file_size;
    pid_t pid;

    if (!check_input_buffer(buffer, client_socket))
        return;
    file_path = buffer[1];
    file = open_file(file_path, client_socket);
    if (!file)
        return;
    file_size = get_file_size(file_path);
    check_size(file_size, client_socket);
    pid = fork();
    if (pid == -1)
        fork_error(client_socket, server->clients[client_socket].data_socket);
    else if (pid == 0)
        handle_child_process(file, file_size, client_socket, server);
    else
        close(server->clients[client_socket].data_socket);
}
