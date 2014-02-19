
PROJECT_NAME		:= libcli++
BASEVERSION			:= 0
LIBVERSION			:= $(BASEVERSION).0.1
BASENAME			:= libcli++
SONAME				:= $(BASENAME).so.$(BASEVERSION)
LIBNAME				:= $(BASENAME).so.$(LIBVERSION)
LINKERNAME			:= $(BASENAME).so
INSTALLDIR			:= /usr/local/lib
INCLUDEINSTALLDIR	:= /usr/local/include
INCLUDEDIRNAME		:= cli++
SVN_REVISION		:= $(shell svnversion .. | sed -e "s/[0-9]*://" -e "s/[MS]//")
GIT_HASH			:= $(shell git rev-parse --short HEAD)

INCLUDEDIR			:= ../include
INCLUDE				:= -I$(INCLUDEDIR) -I$(INCLUDEINSTALLDIR)
LIBNAMEFLAGS		:= -Wl,-soname,$(SONAME)
CXXFLAGS			:= -fPIC -Wall
DEPS_FLAGS			:= -MMD -MP
LIBS				:= 
LIBDIR				:= ../lib
SRCS				:= $(shell find . -type f -name '*.cpp')
OBJS				:= $(SRCS:%.cpp=%.o)
DEPS				:= $(OBJS:%.o=%.d)
VARS				:= -DOS_POSIX -DLIB_REVISION=$(SVN_REVISION) -DGIT_HASH=$(GIT_HASH)
OBJS_WILDCARD		:= *.o
DEPS_WILDCARD		:= *.d

all:
	@echo "$(PROJECT_NAME) library Makefile - Copyright © 2009-2014 Diego Lago <diego.lago.gonzalez@gmail.com>"
	@echo "Git repository at: https://github.com/diegolagoglez/libclipp"
	@echo "Please specify one of the following targets:"
	@echo " debug      : Generates the library in debug mode."
	@echo " release    : Generates an optimized library."
	@echo " install    : Installs generated library into '$(INSTALLDIR)'."
	@echo " devinstall : Installs development files into '$(INCLUDEINSTALLDIR)'."
	@echo " clean      : Cleans generated object files."
	
message:
	@echo "Building project '$(PROJECT_NAME)'..."
	
%.o: %.cpp
	@echo -n "Building '$@'..."
	@$(CXX) -c $(VARS) $(CXXFLAGS) $(DEPS_FLAGS) $(INCLUDE) -o "$@" "$<"
	@echo " OK"

debug: CXXFLAGS := $(CXXFLAGS) -g3 -O0 -DDEBUG
debug: message $(OBJS)
	@echo -n "Generating debug library in '$(LIBDIR)/$(LIBNAME)'..."
	@$(CXX) -shared $(VARS) $(CXXFLAGS) $(LIBS) $(LIBNAMEFLAGS) -o $(LIBDIR)/$(LIBNAME) *.o
	@cd $(LIBDIR) && ln -sf $(LIBNAME) $(LINKERNAME) && ln -sf $(LIBNAME) $(SONAME)
	@echo " OK" 

release: CXXFLAGS := $(CXXFLAGS) -O3 -DRELEASE
release: message $(OBJS)
	@echo -n "Generating release library in '$(LIBDIR)/$(LIBNAME)'..."
	@$(CXX) -shared $(VARS) $(CXXFLAGS) $(LIBS) $(LIBNAMEFLAGS) -o $(LIBDIR)/$(LIBNAME) *.o
	@cd $(LIBDIR) && ln -sf $(LIBNAME) $(LINKERNAME) && ln -sf $(LIBNAME) $(SONAME)
	@echo " OK"

.PHONY: clean install devinstall debug release message

install:
	@echo -n "Installing library '$(LIBNAME)' in '$(INSTALLDIR)'..."
	@install $(LIBDIR)/$(LIBNAME) $(INSTALLDIR)
	@cd $(INSTALLDIR) && ln -sf $(LIBNAME) $(SONAME) && ln -sf $(LIBNAME) $(LINKERNAME)
	@echo " OK"
	
devinstall: install
	@echo -n "Installing development header files in '$(INCLUDEINSTALLDIR)/$(INCLUDEDIRNAME)'..."
	@mkdir -p $(INCLUDEINSTALLDIR)/$(INCLUDEDIRNAME) && cp -r $(INCLUDEDIR)/$(INCLUDEDIRNAME)/* $(INCLUDEINSTALLDIR)/$(INCLUDEDIRNAME)
	@echo " OK"

clean:
	@echo -n "Cleaning project '$(PROJECT_NAME)'..."
	@rm -f $(LIBDIR)/$(LIBNAME) $(LIBDIR)/$(LINKERNAME) $(LIBDIR)/$(SONAME) $(OBJS_WILDCARD) $(DEPS_WILDCARD)
	@echo " OK"

-include $(DEPS)
