
# ft_irc

## About

**ft_irc** is a project aimed at building an IRC (Internet Relay Chat) server from scratch. The project adheres to the RFC 1459 protocol, ensuring compatibility with standard IRC clients. The goal is to implement various IRC features, including handling multiple clients, channels, and basic commands.

## How It Works

The server is written in C++ and uses socket programming to handle client connections. It supports multiple clients simultaneously and provides basic IRC functionalities such as joining channels, sending messages, and managing users.

## Quick Start

### Prerequisites

- C++ compiler (e.g., g++)
- Make

### Steps to Run

1. **Clone the repository:**
   ```sh
   git clone https://github.com/karimomino/ft_irc.git
   cd ft_irc
   ```

2. **Build the project:**
   ```sh
   make
   ```

   This will compile the source code and generate the executable.

3. **Run the IRC server:**
   ```sh
   ./ircserv <port> <password>
   ```

   Replace `<port>` with the port number you want the server to listen on, and `<password>` with the server password.

## Features

- **Client Connections**: Handles multiple clients simultaneously using non-blocking sockets.
- **Channels**: Allows clients to create, join, and leave channels.
- **Messaging**: Supports sending and receiving messages in channels and private messages between clients.
- **Commands**: Implements basic IRC commands such as `NICK`, `USER`, `JOIN`, `PART`, `PRIVMSG`, `QUIT`, etc.

## Contributors
[Omar](https://github.com/ghamry03)
[Lucas](https://github.com/lde-alen)

## Contact

For any questions or issues, please open an issue on GitHub or contact any of the contributors.
