/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"
void check_malloc(Server *server)
{
    if (server->path == NULL) {
        perror("malloc failed for server->path");
        exit(84);
    }
}

void add_path(char *path, Server *server)
{
    char absolute_path[1024];

    if (!realpath(path, absolute_path)) {
        perror("realpath");
        close(server->fd);
        exit(84);
    }
    check_path(absolute_path);
    server->path = malloc(strlen(absolute_path) + 1);
    strcpy(server->path, absolute_path);
    server->cwd = malloc(strlen(absolute_path) + 1);
    if (server->cwd == NULL) {
        perror("malloc failed for server->cwd");
        free(server->path);
        exit(84);
    }
    strcpy(server->cwd, absolute_path);
}

int create_socket(Server *server, struct sockaddr_in client_addr,
    socklen_t client_addrlen)
{
    int new_socket =
        accept(server->fd, (struct sockaddr *)
        &client_addr, &client_addrlen);

    if (new_socket == -1) {
        perror("accept");
        exit(84);
    }
    return new_socket;
}

void new_connection(Server *server)
{
    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    int new_socket = create_socket(server, client_addr, client_addrlen);
    char *req_accepted = "220 \r\n";

    write(new_socket, req_accepted,
        strlen(req_accepted));
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i].fd == -1) {
            server->clients[i].fd = new_socket;
            server->clients[i].address = client_addr;
            server->clients[i].cwd = malloc(strlen(server->cwd) + 1);
            check_cwd(server, i);
            strcpy(server->clients[i].cwd, server->cwd);
            break;
        }
    }
}

void catch_actions(Server *server)
{
    int sd = 0;
    int activity =
        select(server->max_sd + 1, &server->read_fds, NULL, NULL, NULL);

    if ((activity == -1) && (errno != EINTR)) {
        printf("select error\n");
    }
    if (FD_ISSET(server->fd, &server->read_fds)) {
        new_connection(server);
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        sd = server->clients[i].fd;
        if (sd != -1 && FD_ISSET(sd, &server->read_fds)) {
            handle_client(sd, server);
        }
    }
}
