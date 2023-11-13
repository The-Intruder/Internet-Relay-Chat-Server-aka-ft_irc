# ft_irc - Internet Relay Chat Server

![IRC Logo]()

**ft_irc** is a C++98 implementation of an Internet Relay Chat (IRC) server. The project aims to create a fully functional IRC server that allows users to connect, exchange real-time messages, join group channels, and perform various IRC operations.

## Table of Contents

- [Introduction](#introduction)
- [General Rules](#general-rules)
- [Mandatory Part](#mandatory-part)
- [Bonus Part](#bonus-part)
- [Submission and Peer-Evaluation](#submission-and-peer-evaluation)

## Introduction

Internet Relay Chat (IRC) is a text-based communication protocol used for real-time messaging on the Internet. It enables users to participate in group discussions, chat privately, and exchange messages with other connected users. The ft_irc project focuses on developing a server-side implementation of IRC, allowing users to connect to the server using IRC clients and interact with the server and other users.

## General Rules

- The program should be stable and not crash under any circumstances.
- Provide a Makefile for compiling the source code.
- The code must comply with the C++98 standard and compile with the `-std=c++98` flag.
- Avoid using external libraries and Boost libraries.
- Develop using C++ features whenever possible and prefer C++ versions of functions over their C counterparts.

## Mandatory Part

### Program Name: `ircserv`

#### Requirements

- The server should be capable of handling multiple clients simultaneously without hanging.
- Forking is not allowed; all I/O operations must be non-blocking.
- Only one `poll()` (or equivalent) function should be used to handle operations such as read, write, listen, etc.
- Choose a reference IRC client for testing and ensure it can connect to your server without errors.
- Communication between the client and server must be done via TCP/IP (v4 or v6).
- Implement the following basic features:
  - Authentication: Users should be able to authenticate themselves with a password.
  - Nickname and Username: Users should be able to set their nickname and username.
  - Join Channel: Users should be able to join a channel.
  - Private Messages: Users should be able to send and receive private messages.
  - Message Forwarding: Messages sent by one client to a channel should be forwarded to all other clients in the channel.
  - Operators and Regular Users: Implement different privileges for operators and regular users.
  - Operator Commands: Implement the following commands specific to channel operators:
    - KICK: Eject a client from the channel.
    - INVITE: Invite a client to a channel.
    - TOPIC: Change or view the channel topic.
    - MODE: Change the channel's mode (e.g., invite-only, topic restrictions, channel key, operator privileges, user limit).

## Bonus Part

The bonus part includes additional features that can be added to the IRC server to enhance its functionality and make it more similar to an actual IRC server. However, the bonus part will only be evaluated if the mandatory part is implemented perfectly without any malfunction.

- File Transfer: Implement file transfer functionality.
- Bot: Add a bot to the IRC server.

## Submission and Peer-Evaluation

Submit your assignment in your Git repository. Only the work inside your repository will be evaluated during the defense. Make sure to double-check the names of your files before submission.

You are encouraged to create test programs for your project, even though they won't be submitted or graded. These tests can be useful for testing your server during the defense or evaluating other ft_irc projects in the future. You are free to use any necessary tests during the evaluation process.

During the evaluation, your reference client will be used to test the functionality of your IRC server.

For more details, refer to the original project subject document.

**Good luck with your ft_irc project!**
