/*
** EPITECH PROJECT, 2024
** Network
** File description:
** my_ftp
*/

#ifndef FTP_CLIENT_H // Include guard starts
    #define FTP_CLIENT_H

    #include <dirent.h>
    #include <signal.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/errno.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #define MAX_CLIENTS 1000 // Maximum number of clients at any time

    #define UNUSED __attribute__((unused))

// Client structure (example)
typedef struct client_structure {
    int fd;
    struct sockaddr_in address;
    int data_socket;
    // new vars
    bool is_auth;
    bool is_username_correct;
    char *cwd;
} Client;

typedef struct server_structure {
    int fd;
    char *cwd;
    int port;
    int max_sd;
    fd_set read_fds;
    char *path;
    struct sockaddr_in address;
    Client clients[MAX_CLIENTS];
} Server;

typedef void (*command_handler)(int, char **, Server *); // funciton pointer


typedef struct command_structure {
    char *command;
    command_handler handler; // the function pointer
} Command;

// helper struct

typedef struct helper_args {
    char *reply;
    int b_read;
    FILE *file;
    char **read_buffer;
} helper;

void check_args(int argc, char **argv);
int socket_creation(int port, Server *server);
void handle_client(int client_socket, Server *server);

// command handlers
void user_command(int clinent_socket, char **buffer, Server *server);
void pass_command(int clinent_socket, char **buffer, Server *server);
void cwd_command(int client_socket, char **buffer, Server *server);
void pwd_command(int cleint_socket, char **buffer, Server *server);
void noop_command(int client_socket, char **buffer, Server *server);
void quit_command(int client_socket, char **buffer, Server *server);
void cdup_command(int client_socket, char **buffer, Server *server);
void dele_command(int client_socket, char **buffer, Server *server);
void help_command(int client_socket, char **buffer, Server *server);
// file transfer commands
void pasv_command(int client_socket, char **buffer, Server *server);
void port_command(int client_socket, char **buffer, Server *server);
void retr_command(int client_socket, char **buffer, Server *server);
void stor_command(int client_socket, char **buffer, Server *server);
void list_command(int client_socket, char **buffer, Server *server);

//  helpers
void trim_end(char *str);
int create_data_socket(int *port);
int create_active_ds(int port, char *server_ip);
void fork_error(int client_socket, int data_socket);
int get_c(int client_socket, Server *server);

// main_helper
void check_malloc(Server *server);
void check_path(char *path);
void add_path(char *path, Server *server);
void catch_actions(Server *server);
void new_connection(Server *server);
int create_socket(Server *server, struct sockaddr_in client_addr,
    socklen_t client_addrlen);

// process args helper
void init_commands(Command commands[]);
bool is_restricted(char *command, Server *server, int client_socket);
bool is_auth(Server *server, char *command, int client_socket);
void unauth(int client_socket);

// stor helper
bool validate_buffer_and_open_data_socket(char **buffer, int client_socket,
    int data_socket);
void handle_file_transfer(char *file_path, int data_socket, int client_socket);
void close_file_and_data_socket(FILE *file,
    int data_socket, ssize_t bytes_read,
    int client_socket);
void handle_child_process(FILE *file, int file_size, int client_socket,
    Server *server);
void transfer_success(char **reply, int client_socket);
void transfer_error(char **reply, int client_socket);
void opening_message(char **reply, int client_socket);

// retr helper
bool check_input_buffer(char **buffer, int client_socket);
FILE *open_file(char *file_path, int client_socket);
long get_file_size(const char *filename);
void check_size(int file_size, int client_socket);

// port helper
bool check_buffer(char **buffer, int client_socket);
char *get_ip(char *buffer);
int get_port(char *buffer);
void check_socket(int data_socket, int client_socket);

// CDUP helper
bool check_len(Server *server, int client_socket);

// CWD helpers
bool check_dir_and_buffer(DIR **dir, char **reply, int client_socket,
    char **buffer);
bool fill_path(char **buffer, char *resolved_path, char **reply,
    int client_socket);

// LIST helpers
bool check_pipe(FILE *pipe, int client_socket);

// extra helpers
void check_cwd(Server *server, int i);
void check_sd(Server *server, int sd);

#endif // FTP_CLIENT_H // Include guard ends
