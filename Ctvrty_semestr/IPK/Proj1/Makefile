SERVER          = ftrestd
CLIENT          = ftrest

SERVER_OBJECTS  = $(server.cpp=.o)
CLIENT_OBJECTS  = $(client.cpp=.o)

build: $(SERVER) $(CLIENT)

rebuild: clean $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJECTS)
		g++ -g server.cpp -o $@

$(CLIENT): $(CLIENT_OBJECTS)
		g++ -g server.cpp -o $@

clean:
	rm -fr core* *~ $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(SERVER) $(CLIENT) .make.state .sb