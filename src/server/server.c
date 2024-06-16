#include "./server.h"

void print_error(const char *type, const int code)
{
    char error_buffer[1000] = {0};
    sprintf(error_buffer, "'%s' error code: %d", type, code);
    perror(error_buffer);
}

void error_reading(signed char *buffer, int socket, GameState state)
{
    putts("error_reading");
}

void eod(signed char *buffer, int socket, GameState state)
{
    putts("eod");
}

int server_listen(void (*handle_request)(
                      signed char *buffer,
                      int socket,
                      GameState state),
                  GameState state)
{
    int server_fd, new_socket, valread = 1, error_code;
    struct sockaddr_in address;
#if defined(__linux__)
    int
#else
    signed char
#endif
        opt = 1;
    int addrlen = sizeof(address);
    signed char *buffer;

    // Creating socket file descriptor
    assert((server_fd = socket(AF_INET, SOCK_STREAM, 0)) != 0 &&
           "socket creation failed");

    // Forcefully attaching socket to the port
    (error_code = (setsockopt(server_fd,
                              SOL_SOCKET,
                              SO_REUSEADDR
#ifdef _WIN64
                                  | SO_REUSEPORT

#endif
                              ,
                              &opt,
                              sizeof(opt))));

    assert(error_code == 0 && "setsockopt failed");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port
    assert(bind(server_fd, (struct sockaddr *)&address,
                sizeof(address)) >= 0 &&
           "bind failed");

    assert(listen(server_fd, 3) >= 0 && "listen failed");

    putts("Waiting for a client to connect!");

    assert((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen)) >= 0 &&
           "accept failed");

    putts("Client connected");
    while (valread != 0 && valread != -1)
    {
        buffer = calloc(BUFFER_SIZE, sizeof(signed char));
        valread = recv(new_socket, buffer, BUFFER_SIZE, 0);

        bool conditions[3] = {valread == -1, valread == 0, true};
        void (*handle_val_read[3])(signed char *, int, GameState) = {error_reading, eod, handle_request};

        unsigned char index = find_first_true_index(conditions, 3);
        handle_val_read[index](buffer, new_socket, state);

        free(buffer);
    }
    return valread;
}
