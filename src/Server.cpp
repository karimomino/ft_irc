/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 18:41:14 by kamin             #+#    #+#             */
/*   Updated: 2023/08/11 18:38:03 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>

Server::Server(const int port, const std::string pass) : _port(port), _pass(pass)
{
    _hint.sin_family = AF_INET;
    _hint.sin_port = htons(port);
    _hint.sin_addr.s_addr = INADDR_ANY;
    _connectionCount = 0;

    _initCommandsFunctions();

    if (_initServer())
        _runServer();
    else
    {
        perror("Failed to init server.");
    }

    (void)_port;
}

bool Server::_addCommandFunction(std::string const &keyValue, cmdFun funPtr)
{
    if (!funPtr)
        return (false);

    this->_commands.insert(
        std::pair<std::string const, cmdFun>(keyValue, funPtr));

    return (true);
}

bool Server::_initCommandsFunctions(void)
{
    // TODO: add all the commands functions
    // _addCommandFunction( "EXAMPLE", &Server::_exampleCmd );

    return (true);
}

/* KEEEP*/
int Server::_initServer()
{
    int ret = 1;
    int sock_opt = 1;
    pollfd tmp_fd;
    // pollfd  *tmp_fd = new pollfd;

    _listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_listen_socket < 0)
    {
        std::cout << "Error creating socket!" << std::endl;
        ret = 0;
    }
    else if (setsockopt(_listen_socket, SOL_SOCKET, SO_REUSEADDR, &sock_opt,
                        sizeof(sock_opt)))
    {
        std::cout << "Error setting socket options!" << std::endl;
        ret = 0;
    }
    else if (fcntl(_listen_socket, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cout << "Error setting socket to non-blocking!" << std::endl;
        ret = 0;
    }
    else if (bind(_listen_socket, (sockaddr *)&_hint, sizeof(_hint)) < 0)
    {
        std::cout << "Error binding!" << std::endl;
        ret = 0;
    }
    else if (listen(_listen_socket, MAX_CLIENTS))
    {
        std::cout << "Failed to listen" << std::endl;
        ret = 0;
    }
    else if (ret)
    {
        tmp_fd.fd = _listen_socket;
        tmp_fd.events = POLLIN;
        tmp_fd.revents = 0;
        _poll_fds.push_back(tmp_fd);
        _connectionCount = 1;
        struct in_addr ipAddr = _hint.sin_addr;

        std::string ip_string = inet_ntoa(ipAddr);
        this->_ip_string = ip_string;
    }

    return (ret);
}

/* KEEEP*/
void Server::_runServer(void)
{
    // time_t now;

    // Server Loop
    while (true)
    {
        poll(_poll_fds.data(), _poll_fds.size(), 300);

        for (size_t i = 0; i < _poll_fds.size(); i++)
        {

            // if no change on this socket, run next iteration
            if ((_poll_fds.data())[i].revents == 0)
                continue;

            int socket = (_poll_fds.data())[i].fd;

            // now = time(0);
            // this is a new connection
            // TODO: loop to accept all incoming connections
            if (socket == _listen_socket) {
                if (_acceptConnection()) {
                    // Client &cli = _clientMap.find(socket)->second;
                    // if (cli.getPass().compare(_pass)) {
                    //     sendMsg(ERR_PASSWDMISMATCH , cli , "Wrong server password.");
                    //     cli.setTerminate( true );
                    // }
                }
            }

            if ((_poll_fds.data())[i].revents & POLLOUT && socket != _listen_socket)
            {
                Client &cli = _clientMap.find(socket)->second;
                if (cli.getQueueSize() > 0)
                {
                    Client target = cli.getFirstQueuedMsg().first;
                    const std::string finalMsg = cli.getFirstQueuedMsg().second;
                    DEBUG_MSG("SENDING QUEUED MESSAGE: " << finalMsg << std::endl);
                    sendMsg(target, finalMsg);
                    cli.popFirstMsg();
                }

                if (cli.getTerminate())
                    close(cli.getClientSocket());
            }

            if ((_poll_fds.data())[i].revents & POLLIN && socket != _listen_socket)
            {
                std::map<int, Client>::iterator tmp = _getClient(socket);

                // TODO: loop to receive bigger messages
                char buff[1024];
                memset(buff, 0, 1024);
                int rcv = recv(socket, buff, 1024, MSG_DONTWAIT);
                // std::cout << "receiving msg" << std::endl;
                if (rcv > 0)
                {
                    // tm *ltm = localtime(&now);
                    // std::cout << "[ " << 5 + ltm->tm_hour << ":" << ltm->tm_min << ":"
                    //                     << ltm->tm_sec << " ] " << "msg received from client: " << buff << std::endl;
                    // // TODO: Fix hour, its giving the wrong number
                    tmp->second.incMsgSent();
                    _parseMessage(tmp->second, buff);
                    // TODO : check if password matches
                }
                else if (rcv == 0)
                {
                    PollVector::iterator erase_pos = _poll_fds.begin() + (i);
                    _poll_fds.erase(erase_pos);
                    _clientMap.erase(socket);
                    close(socket);
                    std::cout << "client connection closed" << std::endl;
                }
            }
            (_poll_fds.data())[i].revents = 0;
        }
    }
}

std::map<int, Client>::iterator Server::_getClient(const int fd)
{
    return (_clientMap.find(fd));
}

/* KEEEP*/
bool Server::_acceptConnection(void)
{
    bool accepted = true;
    Client newClient = Client(_listen_socket, _hint);
    _clientMap.insert(
        std::pair<int, Client>(newClient.getClientSocket(), newClient));
    pollfd tmp_fd;
    if (_connectionCount < MAX_CLIENTS)
    {
        tmp_fd.fd = newClient.getClientSocket();
        tmp_fd.events = POLLIN | POLLOUT;
        tmp_fd.revents = 0;
        _poll_fds.push_back(tmp_fd);
        _connectionCount++;
    }
    else
    {
        std::cout << "Connection Refused, MAX clients Reached" << std::endl;
        int clientSock = newClient.getClientSocket();
        close(clientSock);
        _clientMap.erase(_clientMap.find(clientSock));
        accepted = false;
    }

    return (accepted);
}

int Server::getListenSocket(void) const { return (this->_listen_socket); }

size_t Server::getConnectionCount(void) const { return (this->_connectionCount); }

void Server::_executeCommand(Client const &client, std::string const &message)
{
    size_t foundSpace = message.find(' ');
    std::string commandType = message.substr(0, foundSpace);

    CmdMap::iterator cmd = _commands.find(commandType);
    // TODO: send an error message to the client
    if (cmd != _commands.end())
    {
        return ;
    }

    cmdFun fun = (cmdFun)(cmd->second);
    (this->*fun)(client, message);
}
/* KEEEP sendMsg*/
bool Server::sendMsg(Client const &client, std::string const &msg) const
{
    ssize_t sendRet = send(client.getClientSocket(), msg.c_str(), msg.length(), MSG_DONTWAIT);
    return (sendRet >= 0 ? true : false);
}
bool Server::sendMsg(Client &client, std::string const &origin, std::string const &msg) const
{
    std::string const finalMsg = origin + msg + "\r\n";
    std::cout << "USERR MSG: [" << finalMsg.substr(0, finalMsg.length() - 2) << "]" << std::endl;
    client.addMsgToQueue(std::pair<Client &, const std::string>(client, finalMsg));
    return (true);
    // return ( send( client.getClientSocket(), finalMsg.c_str(), finalMsg.length(), 0x80 ) );
}

bool Server::sendMsg(Channel &chan, std::string const &origin, std::string const &msg) const
{
    return (chan.sendMsg(*this, origin, msg));
}

bool Server::sendMsg(Channel &chan, std::string const &msg) const
{
    std::vector<Client *> clients = chan.getClients();

    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        send((*it)->getClientSocket(), msg.c_str(), msg.length(), 0x80);
    }
    return (true);
}

bool Server::sendMsg(std::string const &numReply, Client &client, std::string const &msg) const
{
    std::string const finalMsg = ":" + _ip_string + " " + numReply + " " + client.getNick() + " :" + msg + "\r\n";
    client.addMsgToQueue(std::pair<Client &, const std::string>(client, finalMsg));
    return (true);
    // return ( sendMsg( client, finalMsg ) );
}

bool Server::sendMsg(std::string const &numReply, Client &client, std::string const &arg, std::string const &msg) const
{
    std::string const finalMsg = ":" + _ip_string + " " + numReply + " " + client.getNick() + " " + arg + " :" + msg + "\r\n";
    client.addMsgToQueue(std::pair<Client &, const std::string>(client, finalMsg));
    std::cout << "CLIENT MSG: [" << finalMsg.substr(0, finalMsg.length() - 2) << "]" << std::endl;
    return (true);
    // return ( sendMsg( client, finalMsg ) );
}
