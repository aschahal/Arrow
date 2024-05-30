CXX = g++
CXXFLAGS = -Wall -fPIC
LDFLAGS = -shared
LIBRARY_DIR = $(shell pwd)  
TARGET_LIB = libsparrow.so

# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/library/directory

all: export-LD_LIBRARY_PATH $(TARGET_LIB) p1 p2 p3 p4

export-LD_LIBRARY_PATH:
	@echo "Export LD_LIBRARY_PATH with: export LD_LIBRARY_PATH=\$$LD_LIBRARY_PATH:$(LIBRARY_DIR)"

$(TARGET_LIB): sparrow.o
	$(CXX) $(LDFLAGS) -o $@ $^

sparrow.o: sparrow.cpp sparrow.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

p1: p1.o $(TARGET_LIB)
	$(CXX) -o $@ p1.o -L$(LIBRARY_DIR) -lsparrow -Wl,-rpath,$(LIBRARY_DIR)

p2: p2.o $(TARGET_LIB)
	$(CXX) -o $@ p2.o -L$(LIBRARY_DIR) -lsparrow -Wl,-rpath,$(LIBRARY_DIR)

p1.o: p1.cpp
	$(CXX) $(CXXFLAGS) -c p1.cpp -o $@

p2.o: p2.cpp
	$(CXX) $(CXXFLAGS) -c p2.cpp -o $@

p3: p3.o $(TARGET_LIB)
	$(CXX) -o $@ p3.o -L$(LIBRARY_DIR) -lsparrow -Wl,-rpath,$(LIBRARY_DIR)

p3.o: p3.cpp
	$(CXX) $(CXXFLAGS) -c p3.cpp -o $@

p4: p4.o $(TARGET_LIB)
	$(CXX) -o $@ p4.o -L$(LIBRARY_DIR) -lsparrow -Wl,-rpath,$(LIBRARY_DIR)

p4.o: p4.cpp
	$(CXX) $(CXXFLAGS) -c p4.cpp -o $@

clean:
	rm -f *.o *.so p1 p2 p3 p4

.PHONY: all clean export-LD_LIBRARY_PATH
