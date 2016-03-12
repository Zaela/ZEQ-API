
CXX?= g++

CFLAG= 
COPT= -O3 -fomit-frame-pointer -std=c++11 -fpic
CWARN= -Wall -Wextra -Wredundant-decls
CWARNIGNORE= -Wno-unused-result -Wno-strict-aliasing
CINCLUDE=

#ifdef debug
CFLAG+= -O0 -g -Wno-format
BUILDTYPE= debug
#else
#CFLAG+= -DNDEBUG
#BUILDTYPE= release
#endif

DIRBIN= bin/
CINCLUDE+= -Iinclude/

##############################################################################
# Core

SOPATH= $(DIRBIN)libzeq.so
APATH= $(DIRBIN)libzeq.a
HAPI= include/zeq.h

##############################################################################
# Common
##############################################################################
DIRCOMMON= src/common/
BCOMMON= build/$(BUILDTYPE)/common/
_OCOMMON= zeq_struct.o path.o clock.o timer.o timer_pool.o clock.o
_HCOMMON= define.hpp zeq_struct.hpp path.hpp clock.hpp timer.hpp timer_pool.hpp \
 clock.hpp ref_counter.hpp
OCOMMON= $(patsubst %,$(BCOMMON)%,$(_OCOMMON))
HCOMMON= $(patsubst %,$(DIRCOMMON)%,$(_HCOMMON)) $(HAPI)

CINCLUDE+= -I$(DIRCOMMON)

##############################################################################
# Archive
##############################################################################
DIRARCHIVE= src/archive/
BARCHIVE= build/$(BUILDTYPE)/archive/
_OARCHIVE= archive.o file.o pfs.o wld.o conv_material.o conv_model.o \
 wld_model.o wld_material.o conv_vertex_buffer.o
_HARCHIVE= archive.hpp file.hpp pfs.hpp wld.hpp conv_material.hpp conv_model.hpp \
 wld_model.hpp wld_material.hpp fragments.hpp model_listing.hpp \
 conv_vertex_buffer.hpp
OARCHIVE= $(patsubst %,$(BARCHIVE)%,$(_OARCHIVE))
HARCHIVE= $(patsubst %,$(DIRARCHIVE)%,$(_HARCHIVE)) $(HCOMMON)

CINCLUDE+= -I$(DIRARCHIVE)

##############################################################################
# Graphics
##############################################################################
DIRGRAPHICS= src/graphics/
BGRAPHICS= build/$(BUILDTYPE)/graphics/
_OGRAPHICS= vertex_buffer.o opengl.o material.o texture.o anim_texture.o \
  zone_model.o camera.o model_prototype.o model_instance.o model_static.o \
 transformable.o
_HGRAPHICS= vertex.hpp vertex_buffer.hpp opengl.hpp material.hpp texture.hpp \
 anim_texture.hpp zone_model.hpp camera.hpp model_prototype.hpp \
 model_instance.hpp model_static.hpp transformable.hpp
OGRAPHICS= $(patsubst %,$(BGRAPHICS)%,$(_OGRAPHICS))
HGRAPHICS= $(patsubst %,$(DIRGRAPHICS)%,$(_HGRAPHICS)) $(HCOMMON)

DIRMATH= $(DIRGRAPHICS)math/
_HMATH= frustum.hpp mat4.hpp plane.hpp vec3.hpp math.hpp axis_aligned_bounding_box.hpp
HMATH= $(patsubst %,$(DIRMATH)%,$(_HMATH))

CINCLUDE+= -I$(DIRGRAPHICS) -I$(DIRMATH)

##############################################################################
# Window

SOPATHWINDOW= $(DIRBIN)libzeq-window.so
APATHWINDOW= $(DIRBIN)libzeq-window.a
HAPIWINDOW= include/zeq_window.h

##############################################################################
# Window
##############################################################################
DIRWINDOW= src/window/
BWINDOW= build/$(BUILDTYPE)/window/
_OWINDOW= window.o zoneviewer.o
_HWINDOW= define_window.hpp window.hpp zoneviewer.hpp
OWINDOW= $(patsubst %,$(BWINDOW)%,$(_OWINDOW))
HWINDOW= $(patsubst %,$(DIRWINDOW)%,$(_HWINDOW)) $(HAPI) $(HAPIWINDOW)

CINCLUDE+= -I$(DIRWINDOW)

##############################################################################
# Core Linker flags
##############################################################################
LFLAGS= -rdynamic -shared
LSTATIC= 
LDYNAMIC= -lm -ldl -lGL -lfreeimage -pthread
LPATH= -Lbin

##############################################################################
# Window Linker flags
##############################################################################
LDYNAMICWINDOW= -lzeq -lGL -lX11-xcb -lX11 -lxcb -ludev -lfreetype -lxcb-randr \
 -lxcb-dri3 -lxcb-image -pthread -lsfml-window -lsfml-graphics -lsfml-system

##############################################################################
# Build Rules
##############################################################################
Q= @
E= @echo
RM= rm -f 
AR= ar -cfr

##############################################################################
# Generic rules
##############################################################################
.PHONY: default all clean

default all: core window

clean-common:
	$(Q)$(RM) $(BCOMMON)*.o
	$(E) "Cleaned common"

clean-archive:
	$(Q)$(RM) $(BARCHIVE)*.o
	$(E) "Cleaned archive"

clean-graphics:
	$(Q)$(RM) $(BGRAPHICS)*.o
	$(E) "Cleaned graphics"

clean-window:
	$(Q)$(RM) $(BWINDOW)*.o
	$(E) "Cleaned window"

clean-so:
	$(Q)$(RM) $(SOPATH) $(SOPATHWINDOW)
	$(E) "Cleaned shared objects"

clean-a:
	$(Q)$(RM) $(APATH) $(APATHWINDOW)
	$(E) "Cleaned static libraries"

clean: clean-common clean-archive clean-graphics clean-window clean-so clean-a

##############################################################################
# Build rules
##############################################################################
core: $(SOPATH) $(APATH)

window: $(SOPATHWINDOW) $(APATHWINDOW)

$(SOPATH): $(OCOMMON) $(OARCHIVE) $(OGRAPHICS)
	$(E) "Linking $@"
	$(Q)$(CXX) -o $@ $^ $(LSTATIC) $(LPATH) $(LDYNAMIC) $(LFLAGS)

$(APATH): $(OCOMMON) $(OARCHIVE)
	$(E) "Packing $@"
	$(Q)$(AR) $(APATH) $(OCOMMON) $(OARCHIVE)

$(SOPATHWINDOW): $(OWINDOW) $(SOPATH)
	$(E) "Linking $@"
	$(Q)$(CXX) -o $@ $^ $(LSTATIC) $(LPATH) $(LDYNAMICWINDOW) $(LFLAGS)

$(APATHWINDOW): $(OWINDOW)
	$(E) "Packing $@"
	$(Q)$(AR) $(APATHWINDOW) $(OWINDOW)

$(BCOMMON)%.o: $(DIRCOMMON)%.cpp $(HCOMMON) $(HMATH)
	$(E) "CXX       $@"
	$(Q)$(CXX) -c -o $@ $< $(COPT) $(CWARN) $(CWARNIGNORE) $(CFLAGS) $(CINCLUDE)

$(BARCHIVE)%.o: $(DIRARCHIVE)%.cpp $(HARCHIVE) $(HMATH)
	$(E) "CXX       $@"
	$(Q)$(CXX) -c -o $@ $< $(COPT) $(CWARN) $(CWARNIGNORE) $(CFLAGS) $(CINCLUDE)

$(BGRAPHICS)%.o: $(DIRGRAPHICS)%.cpp $(HGRAPHICS) $(HMATH)
	$(E) "CXX       $@"
	$(Q)$(CXX) -c -o $@ $< $(COPT) $(CWARN) $(CWARNIGNORE) $(CFLAGS) $(CINCLUDE)

$(BWINDOW)%.o: $(DIRWINDOW)%.cpp $(HWINDOW)
	$(E) "CXX       $@"
	$(Q)$(CXX) -c -o $@ $< $(COPT) $(CWARN) $(CWARNIGNORE) $(CFLAGS) $(CINCLUDE)

