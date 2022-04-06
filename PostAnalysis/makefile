#------------------------------------------------------------------------------
maindir := $(shell pwd)
SrcSuf := cpp
IncSuf := h
ObjSuf := o
DepSuf := d
DllSuf := so

SrcDir := src
IncDir := include
ObjDir := obj
DepDir := $(ObjDir)

INCDIRS = $(shell pwd)/include $(shell pwd)
EXTRAHDR =
#------------------------------------------------------------------------------
# Architecture to compile for
ARCH          = linux
#ARCH          = solarisCC5


ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)


INCLUDES      = $(ROOTCFLAGS) $(addprefix -I, $(INCDIRS) )
#$(CubaCFLAGS)
CXX           = $(shell root-config --cxx)
LD            = $(shell root-config --ld)
SOFLAGS       = -shared

ifdef DEBUG
  CXXFLAGS    = -g -O0
  LDFLAGS     = -g -O0
  DEFINES     =
else
  CXXFLAGS    = -O3
  LDFLAGS     = -O3
  DEFINES     =
#  DEFINES     = -DNDEBUG
endif

DEFINES      += -DLINUXVERS -DHAS_SSTREAM
ifdef VERBOSE
DEFINES      += -DVERBOSE
endif
ifdef TESTCODE
DEFINES      += -DTESTCODE
endif

CXXFLAGS     += $(DEFINES) $(INCLUDES)
LIBS         += $(ROOTLIBS) $(SYSLIBS) -lpthread
#$(CubaLIBS)
GLIBS        += $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------
SRC = $(wildcard $(SrcDir)/*.$(SrcSuf))
OBJ           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(ObjDir)/%.$(ObjSuf),$(SRC))
DEP           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(DepDir)/%.$(DepSuf),$(SRC))
OBJS          = $(OBJ)
# $(ObjDir)/EachEventDef.$(ObjSuf) $(ObjDir)/TreeReaderDef.$(ObjSuf)

#------------------------------------------------------------------------------

%.x: %.$(SrcSuf) $(OBJS)
	@$(LD) $(LDFLAGS) $(CXXFLAGS) $< -o $@ -L./ $(OBJS) $(GLIBS) $(LIBS)
	@echo "--->$@ done"

$(ObjDir)/%.$(ObjSuf):$(SrcDir)/%.$(SrcSuf)
	@mkdir -p $(ObjDir)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

#-------------------------------------------------------------------------------
clean:
	@rm -rf $(OBJS) *.x
	@echo "--->Clean Done"
