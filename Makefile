CXX=		g++ $(CCFLAGS)
BASELINE=	baseline.o
OBJS =		$(BASELINE) 

LIBS=		-pthread

CCFLAGS= -g

all:		baseline 

baseline:	$(BASELINE)
		$(CXX) -o baseline $(BASELINE) $(LIBS)

clean:
		rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
		rm -f $(OBJS) $(OBJS:.o=.d) baseline 

# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)