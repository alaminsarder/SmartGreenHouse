CC       = g++
CFLAGS   = -std=c++17 -Wall -O2
LDFLAGS  = -lopengl32 -lglu32 -lgdi32 -luser32 -lkernel32

SOURCES  = main.cpp globals.cpp utils.cpp window.cpp \
           sky.cpp greenhouse.cpp plant.cpp sensors.cpp \
           rain.cpp fan.cpp sprinkler.cpp led.cpp dashboard.cpp \
           water.cpp person.cpp

OBJECTS  = $(SOURCES:.cpp=.o)
TARGET   = greenhouse.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q *.o $(TARGET) 2>nul