# définition du compilateur à utiliser :
CC = gcc

# definition des indicateurs de compilation :
CFLAGS = -Wall -Wextra -std=c99

# definition des répertoires dans lesquels chercher les fichiers d'en-tête (.h) :
INCLUDES = -I.

# definition des bibliothèques à lier à l'exécutable :
LDLIBS = -lm

# definition des fichiers sources à compiler :
SRCS = main.c tools.c

# definition des fichiers objets à lier :
OBJS = $(SRCS:.c=.o)

# definition du nom de l'executable :
MAIN = som

all: $(MAIN)
	@echo "$(MAIN) a été compilé !"

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LDLIBS)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(MAIN)