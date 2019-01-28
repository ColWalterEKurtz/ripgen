# GNU General Public License - Version 3.0

CC      := g++
CFLAGS  := --std=c++14 -pedantic -Wall -O2
HEADERS := $(shell find -maxdepth 1 -type f -name "*.h")
SOURCES := $(shell find -maxdepth 1 -type f -name "*.cpp")
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DPFILES := $(patsubst %.cpp,%.d,$(SOURCES))
PROJECT := ripgen
DOXYGEN := doc/html/index.html

# set default target
.DEFAULT_GOAL = $(PROJECT)

# set phony targets
.PHONY: all doc clean

# create binary and documentation
all: $(PROJECT) $(DOXYGEN)

# create documentation
doc: $(DOXYGEN)

# remove producible files
clean:
	@rm -f $(OBJECTS) $(DPFILES) $(PROJECT)
	@rm -rf doc/

# import dependencies (create if missing)
-include $(DPFILES)

# spot dependencies
$(DPFILES): %.d: %.cpp
	$(CC) -MM -o $@ $<

# link object files
$(PROJECT): $(OBJECTS)
	$(CC) -o $(PROJECT) $+

# compile source code
$(OBJECTS): %.o: %.cpp %.d
	$(CC) -c $(CFLAGS) -o $@ $<

# create documentation
$(DOXYGEN): $(HEADERS) $(SOURCES)
	@doxygen $(PROJECT).doxyfile
