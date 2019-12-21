GXX:= g++
GXXFLAGS:= -pedantic -Wall -Wextra -O3 -std=c++17
OBJECTS:= main.o
TARGET:= main.bin
INPUT:= input

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(GXX) $(GXXFLAGS) -o $@ $^

%.o: %.cpp
	$(GXX) $(GXXFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	cat $(INPUT) | ./$(TARGET)
