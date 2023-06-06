# --------------------------- Terminal Color Codes --------------------------- #
NNN  := \033[1;0m
RED := \033[1;31m
GRN := \033[1;32m
BLU := \033[1;34m
GRA := \033[1;37m

# -----------------------------  Project Title ------------------------------- #

define TITLE
${GRN}
          ::::::::::  :::::::::::             :::::::::::   :::::::::     ::::::::
         :+:             :+:                     :+:       :+:    :+:   :+:    :+:
        +:+             +:+                     +:+       +:+    +:+   +:+        
       :#::+::#        +#+                     +#+       +#++:++#:    +#+         
      +#+             +#+                     +#+       +#+    +#+   +#+          
     #+#             #+#                     #+#       #+#    #+#   #+#    #+#    
    ###             ###     ##########  ###########   ###    ###    ########      
${NNN}${GRA}
                                - Created By : ${RED}Mohamed Amine /\\ Hssain /\\ Abdellah${NNN}\n
endef
export TITLE

# ---------------------------------- Variables ------------------------------- #

CC			:= c++
CC_FLAGS	:= -Wall -Wextra -Werror -std=c++98  -fsanitize=address -static-libsan -g

NAME		:= ircserv
MAIN		:= requirements/ircserv.main.cpp
HEADER		:= requirements/ircserv.head.hpp

SRCS_DIR := requirements/srcs/

SRCS_LST := Server.class.cpp Clients.class.cpp  IRCChannels.class.cpp handdle_JOIN.cpp \
			Bot.cpp Misc.functions.cpp PRIVMSG_NOTICE_utils.cpp handdle_PRIVMSG.cpp \
			handdle_NOTICE.cpp handdle_PART.cpp handdle_KICK.cpp Quit.cpp Nick.cpp
SRCS := ${addprefix ${SRCS_DIR}, ${SRCS_LST}}

OBJS_DIR := requirements/objs/
OBJS_LST := ${patsubst %.cpp, %.obj, ${SRCS_LST}}
OBJS := ${addprefix ${OBJS_DIR}, ${OBJS_LST}}

# ---------------------------------- Rules ----------------------------------- #

all: title ${NAME}

${NAME}: ${OBJS_DIR} ${OBJS} ${HEADER} ${MAIN} 
	@${CC} ${CC_FLAGS} ${MAIN} ${OBJS} -o ${NAME}
	@echo "${GRN}Compiled [${NAME}] Successfully!${NNN}"

${OBJS_DIR}%.obj: ${SRCS_DIR}%.cpp ${HEADER}
	@${CC} ${CC_FLAGS} -c $< -o $@
	@echo "${BLU}Compiled $< Successfully!${NNN}"

${OBJS_DIR}:
	@mkdir ${OBJS_DIR}
clean:
	@rm -rf ${OBJS_DIR}
	@echo "${RED}Removed [${OBJS_DIR}] Successfully!${NNN}"

fclean: clean
	@rm -f ${NAME}
	@echo "${RED}Removed [${NAME}] Successfully!${NNN}"

re: fclean all

.PHONY: all clean fclean re title

title:
	@clear
	@echo "$$TITLE"