NAME = minishell

SRCS =  main.c							\
		signal.c						\
		prompt.c						\
		error.c							\
		parse/parse.c					\
		parse/token.c					\
		parse/tokenchecker.c			\
		parse/utils.c					\
		parse/replvar.c					\
		parse/split.c					\
		parse/treebuilder1.c			\
		parse/treebuilder2.c			\
		parse/treedestroyer.c			\
		run/builtin/builtin_cd.c		\
		run/builtin/builtin_echo.c		\
		run/builtin/builtin_env.c		\
		run/builtin/builtin_exit.c		\
		run/builtin/builtin_export.c	\
		run/builtin/builtin_export2.c	\
		run/builtin/builtin_pwd.c		\
		run/builtin/builtin_unset.c		\
		run/builtin/utils.c				\
		run/heredoc/exec.c				\
		run/heredoc/traverse.c			\
		run/handle.c					\
		run/error.c						\
		run/run_signal.c				\
		run/command.c					\
		run/wait.c						\
		run/run.c						\

OBJS = $(SRCS:.c=.o)

INC = ./include

CFLAGS = -Wall -Wextra -Werror

LIBS = -lreadline		\
		Libft/libft.a	\

# LDFLAGS = -L/usr/local/opt/readline/lib
# CPPFLAGS = -I/usr/local/opt/readline/include

#LDFLAGS = -L/opt/homebrew/opt/readline/lib
#CPPFLAGS = -I/opt/homebrew/opt/readline/include

all : $(NAME)

$(NAME) : $(OBJS)
	@$(MAKE) -C ./Libft bonus
	@cc $(LDFLAGS) $^ $(LIBS) -o $(NAME)
	@echo "Minishell"

%.o: %.c
	@echo "Compiling $<"
	@cc $(CFLAGS) $(CPPFLAGS) -c $< -o $@ -I$(INC)
	@tput cuu1; tput el

clean :
	@$(MAKE) -C ./Libft fclean
	@rm -rf $(OBJS)
	@echo "Cleaning Complete!"

fclean : clean
	@rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
