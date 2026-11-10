NAME = minishell
CFLAGS = -Wall -Wextra -Werror
HEADER = -I./include

READLINE_CFLAGS =
READLINE_LIBS = -lreadline
ifeq ($(shell uname -s),Darwin)
  RL_PREFIX := $(shell brew --prefix readline 2>/dev/null)
  ifeq ($(RL_PREFIX),)
    $(error GNU readline requis sur macOS: brew install readline)
  endif
  READLINE_CFLAGS = -I$(RL_PREFIX)/include
  READLINE_LIBS = -L$(RL_PREFIX)/lib -lreadline -Wl,-rpath,$(RL_PREFIX)/lib
endif

SRC = src/main.c src/utils_shell.c \
		src/exec/execution.c src/exec/execution2.c src/exec/pipeline1.c src/exec/pipeline2.c src/exec/redirection.c src/exec/execution_utils.c\
		src/syntax/check_syntax.c src/syntax/syntax_utils.c \
		src/signal/signal.c \
		src/builtins/pwd.c src/builtins/cd.c src/builtins/echo.c src/builtins/env.c src/builtins/export.c\
		src/builtins/exit.c src/builtins/unset.c\
		src/parser/expander.c src/parser/expander_utils.c \
		src/parser/tokenizer.c src/parser/tokenizer_utils.c \
		src/parser/parser.c src/parser/parser_utils.c

OBJ = $(SRC:src/%.c=obj/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft/
	cc $(CFLAGS) $(OBJ) -o $@ libft/libft.a $(READLINE_LIBS)

obj/%.o: src/%.c
	mkdir -p $(dir $@)
	cc $(HEADER) $(READLINE_CFLAGS) -c $< -o $@

clean:
	make clean -C libft/
	rm -rf obj 

fclean: clean
	make fclean -C libft/
	rm -rf $(NAME)

re: fclean all