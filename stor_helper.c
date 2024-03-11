/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"

bool validate_buffer_and_open_data_socket(char **buffer, int client_socket,
    int data_socket)
{
    if (buffer[1] == NULL) {
        write(client_socket, "550 Requested action not taken.\r\n",
            strlen("550 Requested action not taken.\r\n"));
        close(client_socket);
        close(data_socket);
        return false;
    }
    return true;
}

void handle_child_process(FILE *file, int file_size, int client_socket,
    Server *server)
{
    char *reply;
    char *read_buffer;
    int b_read;

    opening_message(&reply, client_socket);
    read_buffer = malloc(file_size + 1);
    if (!read_buffer)
        exit(EXIT_FAILURE);
    b_read = fread(read_buffer, 1, file_size, file);
    if (write(server->clients[client_socket].data_socket,
        read_buffer, b_read) <
        0)
        transfer_error(&reply, client_socket);
    else
        transfer_success(&reply, client_socket);
    free(read_buffer);
    fclose(file);
    close(server->clients[client_socket].data_socket);
    exit(EXIT_SUCCESS);
}

void close_file_and_data_socket(FILE *file, int data_socket,
    ssize_t bytes_read, int client_socket)
{
    fclose(file);
    close(data_socket);
    if (bytes_read < 0) {
        perror("read failed");
        write(client_socket, "550 Error while reading data.\r\n",
            strlen("550 Error while reading data.\r\n"));
        exit(84);
    }
    write(client_socket, "226 Transfer complete.\r\n",
        strlen("226 Transfer complete.\r\n"));
    exit(EXIT_SUCCESS);
}

void handle_file_transfer(char *file_path, int data_socket, int client_socket)
{
    FILE *file = fopen(file_path, "wb");
    char data_buffer[1024];
    ssize_t bytes_read;

    if (!file) {
        perror("fopen failed");
        write(client_socket, "550 Could not open file.\r\n",
            strlen("550 Could not open file.\r\n"));
        exit(EXIT_FAILURE);
    }
    while (bytes_read > 0) {
        bytes_read = read(data_socket,
            data_buffer, sizeof(data_buffer));
        fwrite(data_buffer, 1, bytes_read, file);
    }
    close_file_and_data_socket(file, data_socket, bytes_read, client_socket);
}
