NAME		=	ircserv

SRCS		=	main.cpp					\
				Server.cpp					\
				Client.cpp					\
				Channel.cpp					\
				namespace/utils.cpp			\
				namespace/messages.cpp		\
				command_utils/join.cpp		\
				command_utils/commands.cpp	\
				command_utils/privmsg.cpp	\
				command_utils/pong.cpp		\
				command_utils/kick.cpp		\

SRCS_DIR    =   ./src/

OBJS		=	$(addprefix $(SRCS_DIR), $(SRCS:.cpp=.o))

CXX			=	c++

CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98 -Iincludes # -fsanitize=address

RM			=	rm -f

all			:	$(NAME)

elliot:
	@echo "\033[0;32m"
	@echo " ▄▄▄▄    ▒█████   ███▄    █   ██████  ▒█████   ██▓ ██▀███     ▓█████  ██▓     ██▓     ██▓ ▒█████  ▄▄▄█████▓"
	@echo "▓█████▄ ▒██▒  ██▒ ██ ▀█   █ ▒██    ▒ ▒██▒  ██▒▓██▒▓██ ▒ ██▒   ▓█   ▀ ▓██▒    ▓██▒    ▓██▒▒██▒  ██▒▓  ██▒ ▓▒"
	@echo "▒██▒ ▄██▒██░  ██▒▓██  ▀█ ██▒░ ▓██▄   ▒██░  ██▒▒██▒▓██ ░▄█ ▒   ▒███   ▒██░    ▒██░    ▒██▒▒██░  ██▒▒ ▓██░ ▒░"
	@echo "▒██░█▀  ▒██   ██░▓██▒  ▐▌██▒  ▒   ██▒▒██   ██░░██░▒██▀▀█▄     ▒▓█  ▄ ▒██░    ▒██░    ░██░▒██   ██░░ ▓██▓ ░ "
	@echo "░▓█  ▀█▓░ ████▓▒░▒██░   ▓██░▒██████▒▒░ ████▓▒░░██░░██▓ ▒██▒   ░▒████▒░██████▒░██████▒░██░░ ████▓▒░  ▒██▒ ░ "
	@echo "░▒▓███▀▒░ ▒░▒░▒░ ░ ▒░   ▒ ▒ ▒ ▒▓▒ ▒ ░░ ▒░▒░▒░ ░▓  ░ ▒▓ ░▒▓░   ░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░░▓  ░ ▒░▒░▒░   ▒ ░░   "
	@echo "▒░▒   ░   ░ ▒ ▒░ ░ ░░   ░ ▒░░ ░▒  ░ ░  ░ ▒ ▒░  ▒ ░  ░▒ ░ ▒░    ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░ ▒ ░  ░ ▒ ▒░     ░    "
	@echo " ░    ░ ░ ░ ░ ▒     ░   ░ ░ ░  ░  ░  ░ ░ ░ ▒   ▒ ░  ░░   ░       ░     ░ ░     ░ ░    ▒ ░░ ░ ░ ▒    ░      "
	@echo " ░          ░ ░           ░       ░      ░ ░   ░     ░           ░  ░    ░  ░    ░  ░ ░      ░ ░           "
	@echo "      ░                                                                                                    "
	@echo "\033[0m"

$(NAME)		:	$(OBJS) elliot
				@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)


clean		:
				$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all
