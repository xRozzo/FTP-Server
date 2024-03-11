/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool check_buffer(char **buffer, int client_socket)
{
    if (buffer[1] == NULL) {
        write(client_socket, "550 Requested action not taken\r\n",
            strlen("550 Requested action not taken\r\n"));
        return false;
    }
    return true;
}

int get_port(char *buffer)
{
    char *port = malloc(16);
    int p1 = 0;
    int p2 = 0;
    char *curr_token = strtok(buffer, ",");
    int i = 0;

    port[0] = '\0';
    while (curr_token != NULL) {
        curr_token = strtok(buffer, ",");
        if (i <= 4) {
            continue;
        }
        if (i == 5)
            p1 = atoi(curr_token);
        else
            p2 = atoi(curr_token);
        i++;
    }
    return  (p1 << 8) | p2;;
}

char *get_ip(char *buffer)
{
    char *ip = malloc(4 * 3 + 3 + 1);
    char *curr_token = strtok(buffer, ",");
    int i = 0;
    if (ip == NULL)
        return NULL;
    ip[0] = '\0';
    while (curr_token != NULL && i < 4) {
        if (i > 0) {
            strcat(ip, ".");
        }
        strcat(ip, curr_token);
        curr_token = strtok(NULL, ",");
        i++;
    }
    return ip;
}
void check_socket(int data_socket, int client_socket)
{
    if (data_socket < 0) {
        perror("data_socket fail");
        close(client_socket);
        exit(84);
    }
}
