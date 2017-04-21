TRACEROUTE          = trace

TRACEROUTE_OBJECTS  = $(trace.c=.o)

build: $(TRACEROUTE)

rebuild: clean $(TRACEROUTE)

$(TRACEROUTE): $(TRACEROUTE_OBJECTS)
		gcc -g trace.c -o $@

clean:
	rm -fr core* *~ $(TRACEROUTE_OBJECTS) $(TRACEROUTE) .make.state .sb