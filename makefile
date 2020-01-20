CC		=  g++
OPT		= -O3
DEBUG		= -g
OTHER		= -Wall
CFLAGS		= $(OPT) $(OTHER)
# CFLAGS	= $(DEBUG) $(OTHER)
TARGET		= run
SRCS		= channel.cpp initialization.cpp ue.cpp main_channel.cpp logging_point.cpp
OBJS		= $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

dep :
	gccmakedep $(SRCS)

clean::
	rm -f $(OBJS) *~ $(EXE) core