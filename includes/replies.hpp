#pragma once

#ifndef REPLIES_HPP
# define REPLIES_HPP

# define RPL_INVITING( ip, arg ) ( ":" + ip + " 341 " + (std::string)arg + "\r\n" )

# define ERR_MULITPLEPARAMS( ip, arg ) ( ":" + ip + " 400 " + (std::string)arg + " :Could not process multiple invalid parameters\r\n") // -> cutom NUMERIC_REPLY using ERR_UNKNOWNERROR
# define ERR_NOSUCHNICK( ip, arg ) ( ":" + ip + " 401 " + (std::string)arg + " :No such nick\r\n")
# define ERR_NOSUCHCHANNEL( ip, arg ) ( ":" + ip + " 403 " + (std::string)arg + " :No such channel\r\n")
# define ERR_USERNOTINCHANNEL( ip, arg ) ( ":" + ip + " 441 " + (std::string)arg + " :User not on channel\r\n")
# define ERR_NOTONCHANNEL( ip, arg ) ( ":" + ip + " 442 " + (std::string)arg + " :You're not on channel\r\n")
# define ERR_USERONCHANNEL( ip, arg ) ( ":" + ip + " 443 " + (std::string)arg + " :is already on channel\r\n")
# define ERR_NEEDMOREPARAMS( ip, arg ) ( ":" + ip + " 461 " + (std::string)arg + " :Not enough parameters\r\n")
# define ERR_CHANOPRIVSNEEDED( ip, arg ) ( ":" + ip + " 482 " + (std::string)arg + ":You're not a channel operator\r\n")

#endif
