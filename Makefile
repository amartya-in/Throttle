
MF=Makefile

CXX=g++

CFLAGS=-I$ .
LFLAGS= -L/usr/local/Cellar/boost/1.67.0_1/lib  -llog4cpp \
	   -lboost_date_time -lboost_thread-mt -lboost_system
PREPROC=

EXE=throttle
SRC=main.cpp throttle.cpp

OBJS=$(SRC:.cpp=.o)

.SUFFIXES:
.SUFFIXES:	.cpp .o

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS) 

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)

.cpp.o:
	$(CXX) -MMD $(CFLAGS) $(PREPROC) -c $<
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

$(OBJS): $(MF)

clean:
	rm -f *.o *.d $(EXE) core

run:
	./throttle

