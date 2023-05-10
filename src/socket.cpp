/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:39:21 by kamin             #+#    #+#             */
/*   Updated: 2023/05/10 14:33:44 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <poll.h>

// ON CONNECTION THE BELOW MESSAGE IS SENT TO THE SO_RESOLVER_SIGNATURE
// PASS 123123

// NICK asd

// USER asd 0 * asd

int main(int ac, char  **av)
{
    int d_socket;
    struct sockaddr_in address;
    struct pollfd pfds[1];
    



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
                std::cout << "Waiting for connection ..." << std::endl;
                int new_socket = accept(d_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                if (new_socket >= 0)
                {
                    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&address;
                    struct in_addr ipAddr = pV4Addr->sin_addr;
                    printf("New Connection From: %s\n",inet_ntoa(ipAddr));
                    pfds[0].fd = new_socket;
                    pfds[0].events = POLLIN;
                    
                    // while (1)
                    // {
                        pfds[0].fd = new_socket;
                        pfds[0].events = POLLIN;
                            char buffer[1024] = { 0 };
                        while(poll(pfds, 1, 1000) != -1) { /* error handling elided */
                            // std::cout << "waiting for message..." << std::endl;
                            if(pfds[0].revents & POLLIN) {
                                int val = recv(new_socket, buffer, 1024, MSG_DONTWAIT);
                                printf("%s\n", buffer);
                                memset(buffer, 0, 1024);
                            }
                            if(pfds[0].revents & (POLLERR | POLLHUP)) {
                                printf("Connection From: %s CLOSED!\n",inet_ntoa(ipAddr));
                                pfds[0].fd = -1;
                                exit(0);
                            }
                            // std::cout << "message recieved..." << std::endl;
                        }
                    // }
                    
                    
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