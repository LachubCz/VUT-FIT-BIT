ISASHARK          = isashark

ISASHARK_OBJECTS  = $(isashark.c=.o)

assemble: $(ISASHARK)

reassemble: clean $(ISASHARK)

$(ISASHARK): $(ISASHARK_OBJECTS)
		g++ -g isashark.c -o $@ -lpcap

clean:
	rm -fr core* *~ $(ISASHARK_OBJECTS) $(ISASHARK) .make.state .sb