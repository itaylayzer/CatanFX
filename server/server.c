#include "server.h"

void print_error(const char *type, const int code)
{
    char error_buffer[1000] = {0};
    sprintf(error_buffer, "'%s' error code: %d", type, code);
    perror(error_buffer);
}

int server_listen(void (*handle_request)(
                      signed char *buffer,
                      int socket,
                      GraphPtr graph,
                      unsigned char *harbors, PlayerPtr players,
                      signed char *bankDevelopments,
                      signed char *bankMaterials,
                      const signed char (*store)[TOTAL_MATERIALS],
                      unsigned char *turnOffset,
                      const unsigned char num_of_players),
                  GraphPtr graph,
                  unsigned char *harbors, PlayerPtr players,
                  signed char *bankDevelopments,
                  signed char *bankMaterials,
                  const signed char (*store)[TOTAL_MATERIALS],
                  unsigned char *turnOffset,
                  const unsigned char num_of_players)
{
    int server_fd, new_socket, valread, error_code;
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
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        print_error("bind failed", 0);
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if ((error_code = (setsockopt(server_fd,
                                  SOL_SOCKET,
                                  SO_REUSEADDR
#ifdef _WIN64
                                      | SO_REUSEPORT

#endif
                                  ,
                                  &opt,
                                  sizeof(opt)))))
    {
        print_error("setsockopt", error_code);
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        print_error("bind failed", error_code);
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    putts("Waiting for a client to connect!");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    putts("Server up and running");
    while (1)
    {
        buffer = calloc(BUFFER_SIZE, sizeof(signed char));
        valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
        switch (valread)
        {
        case -1:
            perror("read");
            return 0; // FIXME: exit(EXIT_FAILURE)
            break;
        case 0:
            putts("eod");
            return 0; // exit(EXIT_SUCCESS);
            break;
        default:
            handle_request(buffer,
                           new_socket,
                           graph,
                           harbors,
                           players,
                           bankDevelopments,
                           bankMaterials,
                           store,
                           turnOffset,
                           num_of_players);
        }
        free(buffer);
    }
    return 0;
}
