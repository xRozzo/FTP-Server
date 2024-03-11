/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"
#include <stdbool.h>

bool check_bytes(int bytes_read, int client_socket, char *buffer)
{
    if (bytes_read == -1) {
        printf("error while handling  client request\n");
        fflush(stdout);
        perror("read");
        return false;
    }
    if (bytes_read == 0) {
        printf("Client disconnected\n");
        close(client_socket);
        return false;
    }
    return true;
}

int tokenize_input(char *buffer, char *tokens[], int max_tokens)
{
    int i = 0;
    char *token = strtok(buffer, " ");
    char *temp = NULL;

    trim_end(token);
    while (token != NULL && i < max_tokens) {
        tokens[i++] = token;
        token = strtok(NULL, " ");
        if (token != NULL) {
            trim_end(token);
        }
    }
    tokens[i] = NULL;
    return i;
}

command_handler find_command_handler(char *command, Command commands[])
{
    int res = 0;
    Command *mapping;

    for (mapping = commands; mapping->command != NULL; mapping++) {
        res = strcmp(command, mapping->command);
        if (res == 0) {
            return mapping->handler;
        }
    }
    return NULL;
}

void execute_handler(command_handler handler,
    Server *server, char *command,
    int client_socket, char **tokens)
{
    char *reply;

    if (handler) {
        if (!is_auth(server, command, client_socket)) {
            unauth(client_socket);
            return;
        }
        handler(client_socket, tokens, server);
    } else {
        reply = "500 Command not implemented.\r\n";
        write(client_socket, reply, strlen(reply));
    }
}

void handle_client(int client_socket, Server *server)
{
    char buffer[1024] = {0};
    int bytes_read = read(client_socket, buffer, 1024);
    char *tokens[1024];
    int token_count;
    Command commands[15];
    char *command;
    command_handler handler;

    if (!check_bytes(bytes_read, client_socket, buffer)) {
        return;
    }
    buffer[bytes_read] = '\0';
    token_count = tokenize_input(buffer, tokens, 1024);
    init_commands(commands);
    if (token_count > 0) {
        command = tokens[0];
        handler = find_command_handler(command, commands);
        execute_handler(handler, server, command, client_socket, tokens);
    }
}
