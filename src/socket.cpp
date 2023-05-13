/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:39:21 by kamin             #+#    #+#             */
/*   Updated: 2023/05/12 17:59:22 by kamin            ###   ########.fr       */
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
#include <fcntl.h>

// ON CONNECTION THE BELOW MESSAGE IS SENT TO THE SO_RESOLVER_SIGNATURE
// PASS 123123

// NICK asd

// USER asd 0 * asd

int main(int ac, char  **av)
{
    int d_socket;
    int send_err;
    struct sockaddr_in address;
    struct pollfd pfds[2];
    d_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (d_socket == -1 && !fcntl(d_socket, F_SETFL, O_NONBLOCK))
        std::cout << "Failed to create socket" << std::endl;
    else
    {
        int sock_opt = 1;
        setsockopt(d_socket, SOL_SOCKET, SO_REUSEADDR, &sock_opt,sizeof(sock_opt));
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
                pfds[0].fd = d_socket;
                pfds[0].events = POLLIN;
                pfds[1].fd = d_socket;
                pfds[1].events = POLLIN;
                int new_socket = 0;
                struct sockaddr_in* pV4Addr;
                struct in_addr ipAddr;
                while (poll(pfds, 2, 5000) >= 0)
                {
                    if (new_socket == 0)
                    {
                        new_socket = accept(d_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                        pV4Addr = (struct sockaddr_in*)&address;
                        ipAddr = pV4Addr->sin_addr;
                        printf("New Connection From: %s\n",inet_ntoa(ipAddr));
                        fcntl(new_socket, F_SETFL, O_NONBLOCK);
                        // send_err = send(new_socket, ":nickKooki JOIN #kk\r\n", 22,0x80);
                        pfds[1].fd = new_socket;
                        pfds[1].events = POLLIN;
                    }
                    if (new_socket > 0)
                    {
                        char buffer[1024] = { 0 };
                        if(pfds[1].revents & POLLIN) {
                            // send_err = send(new_socket, ":127.0.0.1 001 nickKooki :123123you there?\r\n", 45,0x80);
                            int val = recv(new_socket, buffer, 1024, MSG_DONTWAIT);
                            printf("buff: %s\n", buffer);
                            printf("%s\n", buffer);
                            char * sss = strdup(":127.0.0.1 001 nickKooki :2you there?\r\n");
                            // connect(new_socket, (sockaddr *)pV4Addr, sizeof(sockaddr *));
                            if (buffer[0] != 0)
                            {
                                printf("sending...");
                                send_err = send(new_socket, sss , 40,0);
                            }
                            memset(buffer, 0, 1024);
                        }
                        if(pfds[1].revents & (POLLERR | POLLHUP)) {
                            printf("Connection CLOSED!\n");
                            pfds[1].fd = -1;
                            exit(0);
                        }
                    }
                    else
                        std::cout << "Failed to accept connection" << std::endl;
                }
            }
        }
        else
            std::cout << "Failed to bind" << std::endl;
    }
    return (0);
}