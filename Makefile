CC = g++
CFLAGS = -Wall
EXEC_NAME = Cforum 
INCLUDES =
LIBS =
OBJ_FILES = main.o Forum.o ApplicationManager.o TcpServer.o SocketManager.o

all : $(EXEC_NAME)

clean:
	rm -f *.o
	rm -f $(EXEC_NAME)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
