
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

INCLUDEDIR		:= ../include
INCLUDE			:= -I$(INCLUDEDIR) -I/usr/local/include
LIBNAMEFLAGS	:= -Wl,-soname,$(SONAME)
CXXFLAGS		:= -fPIC -Wall
LIBS			:= 
LIBDIR			:= ../lib
SRCS			:= $(shell ls *.cpp)
OBJS			:= $(SRCS:%.cpp=%.o)
VARS			:= -DOS_POSIX -DLIB_REVISION=$(SVN_REVISION)

all:
	@echo "'$(PROJECT_NAME)' library Makefile - Copyright © 2009-2011 Diego Lago <diego.lago.gonzalez@gmail.com>"
	@echo " Please specify one of the next targets:"
	@echo " - debug     : Generates the library in debug mode."
	@echo " - release   : Generates an optimized library."
	@echo " - install   : Installs generated library into '/usr/local/lib'."
	@echo " - devinstall: Installs development files into '/usr/local/include'."
	@echo " - clean     : Cleans generated object files."
	
message:
	@echo "Building project '$(PROJECT_NAME)'..."
	
%.o: %.cpp
	@echo -n "Building file '$@' from '$<'..."
	@$(CXX) -c $(VARS) $(CXXFLAGS) $(INCLUDE) -o "$@" "$<"
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
	@rm -f $(LIBDIR)/$(LIBNAME) $(LIBDIR)/$(LINKERNAME) $(LIBDIR)/$(SONAME) *.o
	@echo " OK"

