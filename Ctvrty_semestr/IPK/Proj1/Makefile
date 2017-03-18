SERVER          = ftrestd
CLIENT          = ftrest

SERVER_OBJECTS  = $(server.c=.o)
CLIENT_OBJECTS  = $(client.c=.o)

build: $(SERVER) $(CLIENT)

rebuild: clean $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJECTS)
		g++ -g server.c -o $@

$(CLIENT): $(CLIENT_OBJECTS)
		g++ -g client.c -o $@

clean:
	rm -fr core* *~ $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(SERVER) $(CLIENT) .make.state .sb
