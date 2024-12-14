CC = gcc 
CFLAGS =-c -Wall -g -I. 
LDFLAGS = -lm
EXECUTABLE = VotingMachine StoringData


SOURCES_VOTING = VotingMachine.c
SOURCES_STORING = StoringData.c

OBJECTS_VOTING = $(SOURCES_VOTING:.c=.o)
OBJECTS_STORING = $(SOURCES_STORING:.c=.o)

all: $(EXECUTABLE)

VotingMachine: $(OBJECTS_VOTING)
	$(CC) -o $@ $(OBJECTS_VOTING) $(LDFLAGS)

StoringData: $(OBJECTS_STORING)
	$(CC) -o $@ $(OBJECTS_STORING) $(LDFLAGS)

%.o: %.c 
	$(CC) $(CFLAGS) $< -o $@
	

clean:
	rm -f $(OBJECTS_VOTING) $(OBJECTS_STORING) $(EXECUTABLE)