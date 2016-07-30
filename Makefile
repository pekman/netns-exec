PREFIX = /usr/local
EXEC_PREFIX = $(PREFIX)
BINDIR = $(EXEC_PREFIX)/bin

CC = gcc
CFLAGS = -O2 -Wall
PROG = netns-exec

IPROUTE2_DIR = iproute2
IPROUTE2_OBJS = namespace.o
IPROUTE2_OBJS_FULL = $(addprefix $(IPROUTE2_DIR)/lib/, $(IPROUTE2_OBJS))
IPROUTE2_INCLUDES = -iquote $(IPROUTE2_DIR)/include
IPROUTE2_CFLAGS = $(CFLAGS)

CPPFLAGS += $(IPROUTE2_INCLUDES)

OBJS = $(PROG).o $(IPROUTE2_OBJS_FULL)


all: $(PROG)

$(PROG): $(OBJS)

install: $(PROG)
	install -D -m4755 -t$(BINDIR) $(PROG)
	install -m755 -t$(BINDIR) netns-exec-dbus

uninstall:
	rm -fv $(BINDIR)/$(PROG)

localclean:
	rm -fv $(PROG) $(PROG).o

clean: localclean
	$(MAKE) clean -C $(IPROUTE2_DIR)

distclean: localclean
	$(MAKE) distclean -C $(IPROUTE2_DIR)


# rule for building files in iproute2 subdirectory:
# (This is a bit of a hack. Set some variables to make iproute2's
# Makefile only build what we need and set all needed options.)
$(IPROUTE2_OBJS_FULL):
	$(MAKE) -C $(IPROUTE2_DIR) \
		SUBDIRS='lib' MFLAGS='$(IPROUTE2_OBJS)' \
		EXTRA_CFLAGS='$(IPROUTE2_CFLAGS)'

MAKEOVERRIDES := $(filter-out CFLAGS=% CPPFLAGS=%,$(MAKEOVERRIDES))
