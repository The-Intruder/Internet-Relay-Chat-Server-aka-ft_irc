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
CC_FLAGS	:= -Wall -Wextra -Werror -std=c++98

NAME		:= ircserv
MAIN		:= requirements/ircserv.main.cpp
HEADER		:= requirements/ircserv.head.hpp

SRCS_DIR := requirements/srcs/
SRCS_LST := 
SRCS := ${addprefix ${SRCS_DIR}, ${SRCS_LST}}

OBJS_DIR := requirements/objs/
OBJS_LST := ${patsubst %.cpp, %.obj, ${SRCS_LST}}
OBJS := ${addprefix ${OBJS_DIR}, ${OBJS_LST}}

# ---------------------------------- Rules ----------------------------------- #

.PHONY: all clean fclean re title

all: title ${NAME}

${NAME}: ${OBJS_DIR} ${OBJS} ${HEADER}
	@${CC} ${CC_FLAGS} ${MAIN} ${OBJS} -o ${NAME}
	@echo "${GRN}Compiled [${GRA}${NAME}${GRN}] Successfully!${NNN}"

${OBJS_DIR}%.o: ${SRCS_DIR}%.c ${HEADER}
	@${CC} ${CC_FLAGS} -c $< -o $@
	@echo "${BLU}Compiled${GRA} $< ${BLU}Successfully!${NNN}"

${OBJS_DIR}:
	@mkdir ${OBJS_DIR}
	@echo "${BLU}Created [${GRA}${OBJS_DIR}${BLU}] Successfully!${NNN}"

clean:
	@rm -rf ${OBJS_DIR}
	@echo "${RED}Removed [${GRA}${OBJS_DIR}${RED}] Successfully!${NNN}"

fclean: clean
	@rm -f ${NAME}
	@echo "${RED}Removed [${GRA}${NAME}${RED}] Successfully!${NNN}"

re: fclean all

title:
	@clear
	@echo "$$TITLE"
