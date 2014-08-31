#
# Copyright (C) 1993 Fariborz ``Skip'' Tavakkolian
#
# $Id: Makefile,v 1.4 93/06/13 12:27:30 fst Exp $
#
# Description:
#
# $Log: Makefile,v $
# Revision 1.4  93/06/13  12:27:30  fst
# Added the lint stuff
# 
# Revision 1.3  93/06/06  11:05:53  fst
# Changed CFLAGS to -g for debugging by dmdpi
# 
# Revision 1.2  93/06/05  22:10:42  fst
# Changed the RCS ``Header'' with ``Id''.
# 
# Revision 1.1  93/06/05  22:03:05  fst
# Initial revision
# 

SOURCES = tetris.c drawing.c motion.c
OBJECTS = $(SOURCES:.c=.o)

TARGET = tetris.m
TARGET_LINT = tetris.lint
CC=dmdcc
CFLAGS=-g

$(TARGET_LINT): $(SOURCES)
 lint -I$(DMD)/include $(SOURCES)

$(TARGET): $(OBJECTS)
 $(CC) -o $(TARGET) $(OBJECTS)

dmdtet.shar:
 shar Makefile $(SOURCES) tetris.h > dmdtet.shar

tetris.o: tetris.h
motion.o: tetris.h
