ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: pong.exe

pong.exe: pong.o
	gcc -o pong.exe pong.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

pong.o: pong.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c pong.c

clean:
	del pong.o
	del pong.exe
