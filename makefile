
CC=gcc
CFLAGS=-std=c99 -lm

HDIR=headers
SDIR=source
ODIR=obj

project: $(ODIR)/main.o $(ODIR)/RARaster.o $(ODIR)/RAVector.o $(ODIR)/RATriangle.o $(ODIR)/RAColor.o $(ODIR)/RAMatrix.o $(ODIR)/RAScene.o $(ODIR)/RATriangleGroup.o $(ODIR)/RALight.o
	$(CC) $(CFLAGS) -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<