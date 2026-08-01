TARGET = wvdm.exe
CC     = x86_64-w64-mingw32-gcc

CFLAGS   = -Wall -Wextra -Os -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -nostdlib
LDFLAGS  = -Wl,--gc-sections -Wl,-eWinMain -s -mwindows
LIBS     = -lkernel32 -luser32 -lole32

all: $(TARGET)

$(TARGET): wvdm.c config.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
