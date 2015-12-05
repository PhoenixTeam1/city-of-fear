CC = gcc
CCFLAGS = -Wall -O3 -g
LIBS = -lGL -lglut -lpng -lpthread

SIMULATION_SRC = simulate.c \
		visualization.c \
		block_generator.c \
		populator.c \
		entity.c \
		civilian.c \
		police.c \
		zombie.c

SIMULATION_OBJ = $(SIMULATION_SRC:%.c=%.o)
SIMULATION_EXE = simulate

all: $(SIMULATION_EXE)

$(SIMULATION_EXE) : $(SIMULATION_OBJ)
	$(CC) $(CCFLAGS) $^ -o $@ $(LIBS)

%.o : %.c
	$(CC) $(CCFLAGS) -c $< $(INCLUDES) -o $@

clean:
	rm -rf *.o $(SIMULATION_EXE)
