/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:43:53 by kamin             #+#    #+#             */
/*   Updated: 2023/06/07 10:32:32 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
#include <vector>
#include <string>
#include <algorithm>

typedef std::string string;

#endif


class Channel {
    typedef std::vector< string > StrVector;
    
    private:
        string                 _name;
        string                 _topic;
        string                 _key;
        bool                        _isInviteOnly;
        bool                        _topicOpOnly;
        StrVector  _nicks;
        StrVector  _invitations;
        
        void    _removeInvitation( string );
        


    public:
        Channel( string name , string topic , string key , bool inv , bool top );
        ~Channel( void );
        
        void        setName( string );
        void        setTopicMode( bool );
        void        setInviteMode( bool );

        const string getName( void );
        const string getTopic( void );
        const string getMode( void );

        void    addInvitation( string );
        
        void    addUser( string );

        string	getUsersStr( void );
        StrVector getNicks( void ) const ;
};