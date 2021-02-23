LUA         = 5.2
LUA_CFLAGS  = $(shell pkg-config --cflags lua$(LUA))
LUA_LIBS    = $(shell pkg-config --libs lua$(LUA))
 
CFLAGS      = -O3 -Wall -fPIC $(LUA_CFLAGS)
LIBS        = -lintl $(LUA_LIBS)

PREFIX      = /usr/local
LIBDIR      = $(PREFIX)/lib/lua/$(LUA)

all: intl.so

intl.so: luaintl.o
	$(CC) $(CFLAGS) -shared -o $@ luaintl.o $(LIBS)

clean:
	rm -f *.o *.so

install:
	install -d $(DESTDIR)$(LIBDIR)
	install -m 755 intl.so $(DESTDIR)$(LIBDIR)/intl.so

uninstall:
	rm -f $(DESTDIR)$(LIBDIR)/intl.so

.PHONY: intl.so
