TRACEROUTE          = trace

TRACEROUTE_OBJECTS  = $(traceroute.c=.o)

assemble: $(TRACEROUTE)

reassemble: clean $(TRACEROUTE)

$(TRACEROUTE): $(TRACEROUTE_OBJECTS)
		gcc -g traceroute.c -o $@

clean:
	rm -fr core* *~ $(TRACEROUTE_OBJECTS) $(TRACEROUTE) .make.state .sb
