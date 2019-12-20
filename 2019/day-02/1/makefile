GXX:= g++
GXXFLAGS:= -pedantic -Wall -Wextra -O0 -ggdb -std=c++17
OBJECTS:= main.o
TARGET:= main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(GXX) $(GXXFLAGS) -o $@ $^

%.o: %.cpp
	$(GXX) $(GXXFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
