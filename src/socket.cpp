/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:39:21 by kamin             #+#    #+#             */
/*   Updated: 2023/05/09 15:42:51 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <cstdint>
#include <iostream>

int main(int ac, char  **av)
{
    int d_socket;
    struct sockaddr_in address;

    d_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (d_socket == -1)
        std::cout << "Failed to create socket" << std::endl;
    else
    {
        std::cout << "Succeeded to create socket" << std::endl;
        std::cout << "Binding PORT ..." << std::endl;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(4445);
        if (!bind(d_socket, (struct sockaddr*)&address, sizeof(address)))
        {
            if (listen(d_socket, 3))
                std::cout << "failed to listen on port" << std::endl;
            else
            {
                int addrlen = sizeof(address);
                int new_socket = accept(d_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                if (new_socket >= 0)
                {
                    printf("Main Socket FD: %d\nconnect FD: %d\n",d_socket, new_socket);
                    char buffer[1024] = { 0 };
                    int val = recv(new_socket, buffer, 1024, MSG_WAITALL);
                    printf("%s\n", buffer);
                }
                else
                    std::cout << "Failed to accept connection" << std::endl;
            }
        }
        else
            std::cout << "Failed to bind" << std::endl;
    }
    return (0);
}