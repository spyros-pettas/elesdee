CC = gcc -g 
CFLAGS = -pthread -lglfw -lm
SOURCES = main.c gl3.c matrix.c shader.c obj_loader.c perlin.c control.c \
	    multifractal.c procdl.c gradient.c road.c background.c rply.c ply_loader.c \
	    fbo.c
OBJDIR = ./obj/
OBJECTS =$(addprefix $(OBJDIR), $(SOURCES:.c=.o))
EXECUTABLE = lsd

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)

$(OBJDIR)%.o: %.c
	$(CC) -c $<  -o $@ $(CFLAGS)

clean:
	rm -rf $(OBJDIR)*o lsd
