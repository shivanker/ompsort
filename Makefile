CC  = gcc-4.8
CXX = g++-4.8

CXXFLAGS = -Wall -fopenmp
LIBFLAGS = -fPIC

LFLAGS = -L.
LIBS = -lpsort

SRCS = check.cpp time.cpp
LIBSRCS = rsort.cpp qsort.cpp msort.cpp sort.cpp

OBJS = $(SRCS:.cpp=.o)
LIBOBJS = $(LIBSRCS:.cpp=.o)

LIBRARY = libpsort.so
TEST = check time
.PHONY: clean debug depend

$(LIBRARY): $(LIBOBJS)
	$(CXX) $(CXXFLAGS) -shared -o $(LIBRARY) $(LIBOBJS)

all: $(TEST) $(LIBRARY)
debug: CXXFLAGS += -DDEBUG
debug: all

$(TEST): %: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LFLAGS) $(LIBS)

$(LIBOBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ $(TEST) $(LIBRARY)

depend: $(SRCS) $(LIBSRCS)
	makedepend -Y. $^

# DO NOT DELETE THIS LINE -- make depend needs it

check.o: common.h sort.h payloadsize.h
time.o: common.h sort.h payloadsize.h
msort.o: common.h sort.h payloadsize.h
sort.o: sort.h payloadsize.h
