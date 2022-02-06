CC			= gcc
CFLAGS		= -g -Wall
LDFLAGS		=
OBJFILES	= cshell.o cshelllib.o
TARGET		= cshell

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~