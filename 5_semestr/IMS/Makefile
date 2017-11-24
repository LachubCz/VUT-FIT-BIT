SIMHRID          = simhrid

SIMHRID_OBJECTS  = $(simhrid.c=.o)

assemble: $(SIMHRID)

reassemble: clean $(SIMHRID)

run: reassemble
	@./simhrid

$(SIMHRID): $(SIMHRID_OBJECTS)
		@g++ -g -O2 -o simhrid simhrid.cpp -lsimlib -lm 

clean:
	@rm -fr core* *~ $(SIMHRID_OBJECTS) $(SIMHRID) .make.state .sb