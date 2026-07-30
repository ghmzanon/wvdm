TARGET = vdmanager.exe
CC    ?= x86_64-w64-mingw32-gcc
RC    ?= x86_64-w64-mingw32-windres

CFLAGS   = -O2 -Os -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables
LDFLAGS  = -Wl,--gc-sections -s -mwindows
LIBS     = -lkernel32 -luser32 -lole32

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

size: $(TARGET)
	@ls -lh $(TARGET)
	@$(CC) -MM -MT $(TARGET) main.c 2>/dev/null

clean:
	rm -f $(TARGET) $(TARGET:.exe=.o)

.PHONY: all size clean
