.PHONY: all

SRCDIR=src
OUTDIR=out
DEPDIR=dep

CXX=g++
CXXFLAGS= -I $(SRCDIR) -I $(OUTDIR) --std=c++14
CXXWFLAGS= -Wall -Wextra $(CXXFLAGS)
#SRC=$(wildcard $(SRCDIR)/*.cpp)
SRC= src/main.cpp src/Type.cpp src/BasicCodegen.cpp
OBJ=$(patsubst $(SRCDIR)/%.cpp,$(OUTDIR)/%.o,$(SRC))  $(OUTDIR)/parse.o $(OUTDIR)/lex.o \
			$(OUTDIR)/log/log.y.o $(OUTDIR)/log/log.l.o
DEPF = $(wildcard $(DEPDIR)/*.d)
PARSER = $(OUTDIR)/parse.o
PARSERLOG = $(OUTDIR)/log/log.y.o

all: adtOpti

adtOpti: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

$(OUTDIR)/parse.hpp : $(PARSER)
$(OUTDIR)/log/log.y.hpp : $(PARSERLOG)


$(OUTDIR)/%.o: $(SRCDIR)/%.cpp $(PARSER) $(PARSERLOG)
	@mkdir -p $(OUTDIR)
	@mkdir -p $(DEPDIR)
	$(CXX) $(CXXWFLAGS) -c $< -o $@
	@$(CXX) $(CXXFLAGS) -MM -MT '$@' -MF $(patsubst $(SRCDIR)/%.cpp, $(DEPDIR)/%.d, $<)  $<

$(OUTDIR)/lex.o: $(SRCDIR)/lex.l $(PARSER)
	@mkdir -p $(OUTDIR)
	flex -o $(OUTDIR)/lex.cpp $<
	$(CXX) $(CXXFLAGS) -c $(OUTDIR)/lex.cpp -o $@

$(OUTDIR)/parse.o: $(SRCDIR)/parse.y
	@mkdir -p $(OUTDIR)
	bison -r all -d $< -o $(OUTDIR)/parse.cpp
	$(CXX) $(CXXFLAGS) -c $(OUTDIR)/parse.cpp -o $@

$(OUTDIR)/log/log.l.o: $(SRCDIR)/log.l $(PARSERLOG)
	@mkdir -p $(OUTDIR)/log
	flex -o $(OUTDIR)/log/log.l.cpp $<
	$(CXX) $(CXXFLAGS) -c $(OUTDIR)/log/log.l.cpp -o $@

$(OUTDIR)/log/log.y.o: $(SRCDIR)/log.y
	@mkdir -p $(OUTDIR)/log
	bison -r all -d $< -o $(OUTDIR)/log/log.y.cpp
	$(CXX) $(CXXFLAGS) -c $(OUTDIR)/log/log.y.cpp -o $@

clean:
	rm -rf $(OUTDIR)
	rm -rf $(DEPDIR)

test: all
	./adtOpti < test/tree.typ > test/tree.cpp
	g++ -I lib test/main.cpp -o test/tree.out
	./test/tree.out 2> test/log

include $(DEPF)
