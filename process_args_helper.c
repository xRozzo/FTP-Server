/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"
void init_commands(Command commands[])
{
    commands[0] = (Command){"USER", user_command};
    commands[1] = (Command){"PASS", pass_command};
    commands[2] = (Command){"CWD", cwd_command};
    commands[3] = (Command){"PWD", pwd_command};
    commands[4] = (Command){"NOOP", noop_command};
    commands[5] = (Command){"QUIT", quit_command};
    commands[6] = (Command){"CDUP", cdup_command};
    commands[7] = (Command){"DELE", dele_command};
    commands[8] = (Command){"PORT", port_command};
    commands[9] = (Command){"PASV", pasv_command};
    commands[10] = (Command){"RETR", retr_command};
    commands[11] = (Command){"STOR", stor_command};
    commands[12] = (Command){"LIST", list_command};
    commands[13] = (Command){"HELP", help_command};
    commands[14] = (Command){NULL, NULL};
}

bool is_restricted(char *command, Server *server, int client_socket)
{
    if (strcmp(command, "USER") == 0) {
        return false;
    }
    if (strcmp(command, "QUIT") == 0) {
        return false;
    }
    if (strcmp(command, "PASS") == 0 &&
        server->clients[get_c(client_socket, server)].is_username_correct) {
        return false;
    }
    return true;
}

bool is_auth(Server *server, char *command, int client_socket)
{
    if (!server->clients[get_c(client_socket, server)].is_auth &&
        is_restricted(command, server, client_socket)) {
        return false;
    }
    return true;
}

void unauth(int client_socket)
{
    write(client_socket, "530 unauth\r\n", strlen("530 unauth\r\n"));
}
