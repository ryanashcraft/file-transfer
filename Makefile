# Makefile based off of example from http://content.gpwiki.org/index.php/Makefile

# The compiler
CC = gcc

# Targets
TCP_SERVER = tcpserver
TCP_CLIENT = tcpclient
UDP_SERVER = udpserver
UDP_CLIENT = udpclient
# Default build type (release or debug)
TYPE = debug
# Which directories contain source files
DIRS = src
TCP_CLIENT_DIRS = src/tcp_client
TCP_SERVER_DIRS = src/tcp_server
UDP_CLIENT_DIRS = src/udp_client
UDP_SERVER_DIRS = src/udp_server
# Which directories contain header files
INCS = include 
# Which libraries are linked
LIBS =
# The static libraries to include
SLIBS =
# Where final binaries will go
BIN = bin

# The next blocks change some variables depending on the build type
ifeq ($(TYPE), debug)
CCPARAM = -Wall -Werror -pedantic -std=c99 -g -I$(INCS)
MACROS =
endif

ifeq ($(TYPE), release)
CCPARAM = -Wall -Werror -pedantic -std=c99 -O2 -I$(INCS)
MACROS = NDEBUG
endif

# Add directories to the include and library paths
INCPATH = .
LIBPATH = 

# Which files to add to backups, apart from the source code
EXTRA_FILES = Makefile

# Where to store object and dependancy files.
STORE = .make-$(TYPE)
# Makes a list of the source (.c) files.
SOURCE := $(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.c))
TCP_CLIENT_SOURCE := $(foreach DIR,$(TCP_CLIENT_DIRS),$(wildcard $(DIR)/*.c))
TCP_SERVER_SOURCE := $(foreach DIR,$(TCP_SERVER_DIRS),$(wildcard $(DIR)/*.c))
UDP_CLIENT_SOURCE := $(foreach DIR,$(UDP_CLIENT_DIRS),$(wildcard $(DIR)/*.c))
UDP_SERVER_SOURCE := $(foreach DIR,$(UDP_SERVER_DIRS),$(wildcard $(DIR)/*.c))
# List of header files.
HEADERS := $(foreach DIR,$(INCS),$(wildcard $(INCS)/*.h))
# Makes a list of the object files that will have to be created.
OBJECTS := $(addprefix $(STORE)/, $(SOURCE:.c=.o))
# Makes a list of the object files that will have to be created.
TCP_CLIENT_OBJECTS := $(addprefix $(STORE)/, $(TCP_CLIENT_SOURCE:.c=.o))
TCP_SERVER_OBJECTS := $(addprefix $(STORE)/, $(TCP_SERVER_SOURCE:.c=.o))
UDP_CLIENT_OBJECTS := $(addprefix $(STORE)/, $(UDP_CLIENT_SOURCE:.c=.o))
UDP_SERVER_OBJECTS := $(addprefix $(STORE)/, $(UDP_SERVER_SOURCE:.c=.o))
# Same for the .d (dependancy) files.
DFILES := $(addprefix $(STORE)/,$(SOURCE:.c=.d))

# Specify phony rules. These are rules that are not real files.
.PHONY: clean backup dirs

$(TCP_CLIENT): dirs $(OBJECTS) $(TCP_CLIENT_OBJECTS)
		@echo Linking $(TCP_CLIENT).
		@$(CC) -o $(BIN)/$(TCP_CLIENT) $(OBJECTS) $(TCP_CLIENT_OBJECTS) $(SLIBS) $(LDPARAM) $(foreach LIBRARY, \
			$(LIBS),-l$(LIBRARY)) $(foreach LIB,$(LIBPATH),-L$(LIB))

$(TCP_SERVER): dirs $(OBJECTS) $(TCP_SERVER_OBJECTS)
		@echo Linking $(TCP_SERVER).
		@$(CC) -o $(BIN)/$(TCP_SERVER) $(OBJECTS) $(TCP_SERVER_OBJECTS) $(SLIBS) $(LDPARAM) $(foreach LIBRARY, \
			$(LIBS),-l$(LIBRARY)) $(foreach LIB,$(LIBPATH),-L$(LIB))

$(UDP_CLIENT): dirs $(OBJECTS) $(UDP_CLIENT_OBJECTS)
		@echo Linking $(UDP_CLIENT).
		@$(CC) -o $(BIN)/$(UDP_CLIENT) $(OBJECTS) $(UDP_CLIENT_OBJECTS) $(SLIBS) $(LDPARAM) $(foreach LIBRARY, \
			$(LIBS),-l$(LIBRARY)) $(foreach LIB,$(LIBPATH),-L$(LIB))

$(UDP_SERVER): dirs $(OBJECTS) $(UDP_SERVER_OBJECTS)
		@echo Linking $(UDP_SERVER).
		@$(CC) -o $(BIN)/$(UDP_SERVER) $(OBJECTS) $(UDP_SERVER_OBJECTS) $(SLIBS) $(LDPARAM) $(foreach LIBRARY, \
			$(LIBS),-l$(LIBRARY)) $(foreach LIB,$(LIBPATH),-L$(LIB))

# Rule for creating object file and .d file, the sed magic is to add
# the object path at the start of the file because the files gcc
# outputs assume it will be in the same dir as the source file.
$(STORE)/%.o: %.c
		@echo Creating object file for $*...
		@$(CC) -Wp,-MMD,$(STORE)/$*.dd $(CCPARAM) $(foreach INC,$(INCPATH),-I$(INC))\
				$(foreach MACRO,$(MACROS),-D$(MACRO)) -c $< -o $@
		@sed -e '1s/^\(.*\)$$/$(subst /,\/,$(dir $@))\1/' $(STORE)/$*.dd > $(STORE)/$*.d
		@rm -f $(STORE)/$*.dd

# Empty rule to prevent problems when a header is deleted.
%.h: ;

# Cleans up the objects, .d files and executables.
clean:
		@echo Making clean.
		@-rm -f $(foreach DIR,$(DIRS),$(STORE)/$(DIR)/*.d $(STORE)/$(DIR)/*.o)
		@-rm -f $(TARGET)
		@-rm $(BIN)/*

# Backup the source files.
backup:
		@-if [ ! -e .backup ]; then mkdir .backup; fi;
		@zip .backup/backup_`date +%d-%m-%y_%H.%M`.zip $(SOURCE) $(HEADERS) $(EXTRA_FILES)

# Create necessary directories
dirs:
		@-if [ ! -e $(STORE) ]; then mkdir $(STORE); fi;
		@-$(foreach DIR,$(DIRS), if [ ! -e $(STORE)/$(DIR) ]; \
		 then mkdir $(STORE)/$(DIR); fi; )
		@-$(foreach DIR,$(TCP_CLIENT_DIRS), if [ ! -e $(STORE)/$(DIR) ]; \
		 then mkdir $(STORE)/$(DIR); fi; )
		@-$(foreach DIR,$(TCP_SERVER_DIRS), if [ ! -e $(STORE)/$(DIR) ]; \
		 then mkdir $(STORE)/$(DIR); fi; )
		@-$(foreach DIR,$(UDP_CLIENT_DIRS), if [ ! -e $(STORE)/$(DIR) ]; \
		 then mkdir $(STORE)/$(DIR); fi; )
		@-$(foreach DIR,$(UDP_SERVER_DIRS), if [ ! -e $(STORE)/$(DIR) ]; \
		 then mkdir $(STORE)/$(DIR); fi; )
		@-$(foreach DIR,$(INCS), if [ ! -e $(STORE)/$(DIR) ]; \
		 then mkdir $(STORE)/$(DIR); fi; )

all: tcpclient tcpserver udpclient udpserver

# Includes the .d files so it knows the exact dependencies for every
# source.
-include $(DFILES)$(target) $(bins) 
