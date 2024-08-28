NAME      := rest-db-ws
VERSION   := 0.1c
LIBS      := -lmicrohttpd -lmongoc-1.0 -lbson-1.0 -lm ${EXTRA_LIBS} 
TARGET	  := $(NAME)
SOURCES   := $(shell find src/ -type f -name *.c)
OBJECTS   := $(patsubst src/%,build/%,$(SOURCES:.c=.o))
DEPS	  := $(OBJECTS:.o=.deps)
DESTDIR   := /
PREFIX    := /usr
BINDIR    := $(DESTDIR)/$(PREFIX)/bin
CFLAGS    := -O3 -fpie -fpic -Wall -DNAME=\"$(NAME)\" -DVERSION=\"$(VERSION)\" -DPREFIX=\"$(PREFIX)\" ${EXTRA_CFLAGS} -ffunction-sections -fdata-sections

LDFLAGS := -s -pie -Wl,--gc-sections ${EXTRA_LDFLAGS}

$(TARGET): $(OBJECTS) 
	echo $(SOURCES)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

build/%.o: src/%.c
	@mkdir -p build/
	$(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $<

clean:
	$(RM) -r build/ $(TARGET)

install: $(TARGET)
	mkdir -p $(BINDIR) 
	strip $(TARGET)
	install -m 755 $(TARGET) ${BINDIR}

-include $(DEPS)

.PHONY: clean

