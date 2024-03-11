/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"
void set_clients(Server *server)
{
    int sd = 0;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        sd = server->clients[i].fd;
        if (sd != -1) {
            FD_SET(sd, &server->read_fds);
            check_sd(server, sd);
        }
    }
}

void main_loop(Server *server)
{
    int activity = 0;

    while (1) {
        FD_ZERO(&server->read_fds);
        FD_SET(server->fd, &server->read_fds);
        server->max_sd = server->fd;
        set_clients(server);
        catch_actions(server);
    }
}

void check_path(char *path)
{
    DIR *dir;

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir:");
        exit(84);
    }
}

void configure_server(Server *server, int argc, char **argv)
{
    server->fd = 0;
    server->max_sd = server->fd;
    memset(&server->clients, 0, sizeof(server->clients));
    memset(&server->port, 0, sizeof(argv[1]));
    memset(&server->path, 0, sizeof(argv[2]));
    server->port = atoi(argv[1]);
    add_path(argv[2], server);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->clients[i].fd = -1;
    }
    server->fd = socket_creation(server->port, server);
    if (server->fd == -1) {
        perror("socket_creation");
        exit(84);
    }
}

int main(int argc, char **argv)
{
    Server server;

    check_args(argc, argv);
    memset(&server, 0, sizeof(server));
    configure_server(&server, argc, argv);
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        perror("Failed to set SIGCHLD to SIG_IGN");
        exit(EXIT_FAILURE);
    }
    main_loop(&server);
    return 0;
}
