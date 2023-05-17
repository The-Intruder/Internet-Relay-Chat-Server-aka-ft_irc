/* -------------------------------------------------------------------------- */
/*  File: Channel.class.hpp                                                   */
/*  Brief: Channel class header file                                          */
/*  Authors:                                                                  */
/*   - Mohamed Amine Naimi                                                    */
/* -------------------------------------------------------------------------- */

#pragma once
#ifndef CHANNEL_CLASS_HPP
# define CHANNEL_CLASS_HPP

/* ---------------------------------- INCLUDES ------------------------------ */

# include "Libraries.includes.hpp"

/* ---------------------------------- CLASSES ------------------------------- */

typedef struct Client {
    int i;
} Client;

class Channel
{
    private:
        std::string                     _name;
        std::string                     _topic;
        std::string                     _key;
        time_t                          _creation_time;
        time_t                          _last_active;
        uint64_t                        _client_limit;
    public:
        uint16_t                        _modes; 
        std::map<uint16_t, Client>      _clients;
        std::map<uint16_t, Client>      _banned_clients;

    public:
        Channel();
        Channel(std::string name, std::string topic, std::string key, uint64_t client_limit);
        Channel(Channel const &src);
        const Channel &operator=(Channel const &src);
        ~Channel();

        void    setName(std::string name);
        void    setTopic(std::string topic);
        void    setKey(std::string key);
        void    setCreationTime();
        void    setLastActiveTime();
        void    setClientLimit(uint64_t _client_limit);

        bool    isPrivate() const;
        bool    isSecret() const;
        bool    isInviteOnly() const;
        bool    isOnlyVoiceAndOps() const;
        bool    isNoOutsideMessages() const;
        bool    isOnlyOpsChangeTopic() const;

        void    addClient(uint16_t client_id, Client client);
        void    removeClient(uint16_t client_id);
        void    banClient(uint16_t client_id, Client client);
};


#endif // !CHANNEL_CLASS_HPP
