TARGET = wvdm.exe
CC    ?= x86_64-w64-mingw32-gcc
RC    ?= x86_64-w64-mingw32-windres

CFLAGS   = -Wall -Wextra -O2 -Os -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -nostdlib
LDFLAGS  = -Wl,--gc-sections -Wl,-eWinMain -s -mwindows
LIBS     = -lkernel32 -luser32 -lole32

all: $(TARGET)

$(TARGET): wvdm.c config.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

size: $(TARGET)
	@ls -lh $(TARGET)
	@$(CC) -MM -MT $(TARGET) wvdm.c 2>/dev/null

clean:
	rm -f $(TARGET) $(TARGET:.exe=.o)

.PHONY: all size clean
