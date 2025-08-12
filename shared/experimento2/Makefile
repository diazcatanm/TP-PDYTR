CC = gcc
CFLAGS = -Wall -g
SRC_DIR = code
EXEC_DIR = exec

EXECUTABLES = cliente_v1 cliente_v2 cliente_v3 servidor_v1 servidor_v2
TARGETS = $(addprefix $(EXEC_DIR)/, $(EXECUTABLES))
SOURCES = $(addprefix $(SRC_DIR)/, $(addsuffix .c, $(EXECUTABLES)))

all: $(EXEC_DIR) $(TARGETS)

$(EXEC_DIR):
	mkdir -p $(EXEC_DIR)

# Regla general para compilar cualquier .c en ./code a ejecutable en ./exec
$(EXEC_DIR)/%: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

