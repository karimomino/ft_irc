NAME		=	ircserv

SRCS		=	Server.cpp \
				AClient.cpp \
				Channel.cpp \
				Client.cpp \
				main.cpp \
				PreClient.cpp \
				utils.cpp \
				Commands/ICommand.cpp \
				Commands/Kick.cpp \
				Commands/Invite.cpp \
				Commands/Join.cpp \
				Commands/Mode.cpp \
				Commands/Pass.cpp \
				Commands/User.cpp \
				Commands/Nick.cpp \
				Commands/Topic.cpp \
				Commands/PrivMsg.cpp \
				Commands/Notice.cpp \
				Commands/Ping.cpp 

HEADERS		=	includes/Server.hpp \
				includes/AClient.hpp \
				includes/Channel.hpp \
				includes/Client.hpp \
				includes/PreClient.hpp \
				includes/utils.hpp \
				includes/Commands/ICommand.hpp \
				includes/Commands/Kick.hpp \
				includes/Commands/Pass.hpp \
				includes/Commands/User.hpp \
				includes/Commands/Nick.hpp \
				includes/Commands/PrivMsg.hpp \
				includes/Commands/Notice.hpp \
				includes/Commands/Ping.hpp \
				includes/replies.hpp

SRCS_DIR    =   ./src/

OBJS		=	$(addprefix $(SRCS_DIR), $(SRCS:.cpp=.o))

CXX			=	c++

CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98 -Iincludes

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

$(NAME)		:	$(OBJS) $(HEADERS) elliot
				@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
				
debug		:	CXXFLAGS+= -DDEBUG -g3

debug		:	fclean $(NAME)

clean		:
				$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all
