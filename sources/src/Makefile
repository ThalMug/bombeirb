#-------------------------------------------------------------------------------
# This file is part of Bombeirb.
# Copyright (C) 2018 by Laurent Réveillère
#-------------------------------------------------------------------------------
CC = gcc

ifneq ($(config), win)
	SDLCFLAGS = `sdl-config  --cflags`
	SDLLDFLAGS = `sdl-config --libs`
else
	SDLLDFLAGS = -lmingw32 -lSDLmain -lSDL
	SDLCFLAGS = 
endif 
ifneq ($(DEBUG), 0)
	CFLAGS = -Wall -O0 -g -D DEBUG -std=c99 -I../include/ $(SDLCFLAGS)
else
	CFLAGS = -Wall -std=c99 -I../include/ $(SDLCFLAGS)
endif

LDFLAGS = $(SDLLDFLAGS) -lSDL_image
DEPFLAGS = -I ../include 

SRC  = $(wildcard $(SRCDIR)/*.c)
OBJ  = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

.PHONY: all 
all : $(BINDIR)/$(EXEC) 

ifndef SRCDIR
	$(error SRCDIR is not set)
endif

ifndef OBJDIR
	$(error OBJDIR is not set)
endif

ifndef BINDIR
	$(error BINDIR is not set)
endif

ifndef EXEC
	$(error EXEC is not set)
endif

$(BINDIR)/$(EXEC) : $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(OBJDIR)/%.d
	$(CC) -c $(CFLAGS) -o $@ $< 

$(OBJDIR)/%.d : $(SRCDIR)/%.c 
	$(CC) -MM $(DEPFLAGS) $< > $@

-include $(SRC:%.c=%.d)
