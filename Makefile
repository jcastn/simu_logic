# ----------------------------
#	INGREDIENTS
# ----------------------------

NAME		:= simu_logic_app
DEBUG_NAME	:= simu_logic_debug

OBJ_DIR 	:= build

SRCS		:= \
	src/cli/commands.c \
	src/cli/commands-circuit.c \
	src/cli/commands-component.c \
	src/cli/commands-help.c \
	src/cli/commands-link.c \
	src/cli/commands-list.c \
	src/cli/commands-map.c \
	src/cli/console-output.c \
	src/cli/run-loop.c \
	src/cli/user-entry.c \
	src/core/circuits.c \
	src/core/components.c \
	src/core/eval.c \
	src/core/helper.c \
	src/core/in-out.c \
	src/core/links.c \
	src/core/models.c \
	src/core/rearrange-circuit.c \
	src/main.c \
	third_party/linenoise/linenoise.c \
	third_party/tinyfiledialogs/tinyfiledialogs.c \


OBJS		:= $(SRCS:%.c=$(OBJ_DIR)/%.o)
	

# ----------------------------
#	UTENSILS
# ----------------------------

# Compiler and flags
CC		:= gcc
INCLUDES	:= -Iinclude -Ithird_party
CFLAGS		:= -Wall -Wextra -Werror $(INCLUDES)

# Bash commands
RM		:= rm -rf
DIR_DUP		= mkdir -p $(@D)

# ----------------------------
#	RECIPES 
# ----------------------------

all: $(NAME)

# If debug mode, add debug flags
debug : fclean
	@$(MAKE) --no-print-directory $(DEBUG_NAME) CFLAGS="$(CFLAGS) -g3 -DDEBUG_MODE"

# Compile app in default mode
$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

# Compile app in debug mode
$(DEBUG_NAME): $(OBJS)
	$(CC) $(OBJS) -o $(DEBUG_NAME)

# Shutup warns from third-party
$(OBJ_DIR)/third_party/%.o: third_party/%.c
	@$(DIR_DUP)
	$(CC) -w -c -o $@ $<

# Compile obejcts
$(OBJ_DIR)/%.o: %.c
	@$(DIR_DUP)
	$(CC) $(CFLAGS) -c -o $@ $<

# Remove build folder
clean:
	$(RM) $(OBJ_DIR)

# Remove app and build folder
fclean: clean
	$(RM) $(NAME) $(DEBUG_NAME)

# Clean and regenerate app
re: fclean all

.PHONY: all debug clean fclean re
