.PHONY: all

SRCDIR=src
OUTDIR=out
DEPDIR=dep

CXX=g++
CXXFLAGS= -I $(SRCDIR) -I $(OUTDIR)
CXXWFLAGS= -Wall -Wextra -Werror $(CXXFLAGS)
SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(patsubst $(SRCDIR)/%.cpp,$(OUTDIR)/%.o,$(SRC))  $(OUTDIR)/parse.o $(OUTDIR)/lex.o
DEPF = $(wildcard $(DEPDIR)/*.d)


all: adtOpti

adtOpti: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

$(OUTDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OUTDIR)
	@mkdir -p $(DEPDIR)
	$(CXX) $(CXXWFLAGS) -c $< -o $@
	@$(CXX) -MM -MT '$@' -MF $(patsubst $(SRCDIR)/%.cpp, $(DEPDIR)/%.d, $<)  $<

$(OUTDIR)/lex.o: $(SRCDIR)/lex.l
	flex -o $(OUTDIR)/lex.cpp $<
	$(CXX) $(CXXFLAGS) -c $(OUTDIR)/lex.cpp -o $@

$(OUTDIR)/parse.o: $(SRCDIR)/parse.y
	bison -r all -d $< -o $(OUTDIR)/parse.cpp
	$(CXX) $(CXXFLAGS) -c $(OUTDIR)/parse.cpp -o $@

clean:
	rm -rf $(OUTDIR)
	rm -rf $(DEPDIR)

include $(DEPF)
