#include  "../include/socket_utils.h"

int initialize_server(struct sockaddr_in* address)
{
    int server_fd;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0 );
    if (server_fd == 0)
	{
        perror("Socket failed!");
        return (-1);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof (opt)))
	{
        perror("setsockopt failed");
        close(server_fd);
        return (-1);
    }
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);
    if (bind(server_fd, ( struct sockaddr*)address, sizeof (*address)) < 0)
	{
        perror("Bind failed!");
        close(server_fd);
        return (-1);
    }
    if (listen(server_fd, 3 ) < 0)
	{
        perror("Listen failed!");
        close(server_fd);
        return (-1);
    }

    return (server_fd);
}
