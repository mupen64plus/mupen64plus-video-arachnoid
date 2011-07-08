#/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# *   Arachnoid - Makefile                                                  *
# *   http://bitbucket.org/wahrhaft/mupen64plus-video-arachnoid/            *
# *   Copyright (C) 2009 Richard42, Jon Ring                                *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# Makefile for mupen64plus-video-arachnoid

# detect operation system
UNAME = $(shell uname -s)
ifeq ("$(UNAME)","Linux")
  OS = LINUX
  SO_EXTENSION = so
  SHARED = -shared
endif
ifeq ("$(UNAME)","linux")
  OS = LINUX
  SO_EXTENSION = so
  SHARED = -shared
endif
ifneq ("$(filter GNU hurd,$(UNAME))","")
  OS = LINUX
  SO_EXTENSION = so
  SHARED = -shared
endif
ifeq ("$(UNAME)","Darwin")
  OS = OSX
  LDLIBS += -liconv -lpng
  SO_EXTENSION = dylib
  SHARED = -bundle
endif
ifeq ("$(UNAME)","FreeBSD")
  OS = FREEBSD
  SO_EXTENSION = so
  SHARED = -shared
endif
ifeq ("$(UNAME)","OpenBSD")
  OS = FREEBSD
  SO_EXTENSION = so
  SHARED = -shared
  $(warning OS type "$(UNAME)" not officially supported.')
endif
ifneq ("$(filter GNU/kFreeBSD kfreebsd,$(UNAME))","")
  OS = LINUX
  SO_EXTENSION = so
  SHARED = -shared
endif

# detect system architecture
HOST_CPU ?= $(shell uname -m)
NO_ASM ?= 1
ifneq ("$(filter x86_64 amd64,$(HOST_CPU))","")
  CPU := X86
  ifeq ("$(BITS)", "32")
    ARCH_DETECTED := 64BITS_32
  else
    ARCH_DETECTED := 64BITS
  endif
endif
ifneq ("$(filter pentium i%86,$(HOST_CPU))","")
  CPU := X86
  ARCH_DETECTED := 32BITS
endif
ifneq ("$(filter ppc powerpc,$(HOST_CPU))","")
  CPU := PPC
  ARCH_DETECTED := 32BITS
endif
ifneq ("$(filter ppc64 powerpc64,$(HOST_CPU))","")
  CPU := PPC
  ARCH_DETECTED := 64BITS
endif

# base CFLAGS, LDLIBS, and LDFLAGS
OPTFLAGS ?= -O3
CFLAGS += $(OPTFLAGS) -ffast-math -fno-strict-aliasing -fvisibility=hidden -I../../src \
		 -I../../src/hash -I../../src/ucodes -I../../src/GBI -I../../src/RDP -I../../src/utils \
		 -I../../src/log -I../../src/RSP -I../../src/framebuffer -I../../src/math -I../../src/renderer \
		 -I../../src/Assembler -I../../src/texture -I../../src/config -I../../src/Combiner 
CXXFLAGS += -fvisibility-inlines-hidden
LDFLAGS += $(SHARED)

# On OS X, add a few extra flags to elegantly support cross-compilation and backward
# compatibility (and also the flags to link against OpenGL)
ifeq ($(OS), OSX)
  ifeq ($(CPU), X86)
    ifeq ($(ARCH_DETECTED), 64BITS)
      CFLAGS += -arch x86_64 -mmacosx-version-min=10.5 -isysroot /Developer/SDKs/MacOSX10.5.sdk
      LDFLAGS += -bundle -framework OpenGL -arch x86_64 -mmacosx-version-min=10.4 -isysroot /Developer/SDKs/MacOSX10.5.sdk
    else
      CFLAGS += -arch i686 -mmacosx-version-min=10.5 -isysroot /Developer/SDKs/MacOSX10.5.sdk
      LDFLAGS += -bundle -framework OpenGL -arch i686 -mmacosx-version-min=10.4 -isysroot /Developer/SDKs/MacOSX10.5.sdk
    endif
  endif
else
  # test for essential build dependencies
  ifeq ($(shell which pkg-config 2>/dev/null),)
    $(error pkg-config not found)
  endif
  ifeq ($(shell pkg-config --modversion gl 2>/dev/null),)
    $(error No OpenGL development libraries found!)
  endif
  ifeq ($(shell pkg-config --modversion glu 2>/dev/null),)
    $(error No OpenGL utility development libraries found!)
  endif

  CFLAGS += -pthread $(shell pkg-config --cflags gl glu)
  LDLIBS += -pthread $(shell pkg-config --libs gl glu)
endif
ifeq ($(OS), LINUX)
  LDLIBS += -ldl 
endif
ifeq ($(OS), FREEBSD)
  LDLIBS += -lc
endif


ifeq ($(ARCH_DETECTED), 64BITS)
  CFLAGS += -fPIC
endif

# set mupen64plus core API header path
ifneq ("$(APIDIR)","")
  CFLAGS += "-I$(APIDIR)"
else
  TRYDIR = ../../../mupen64plus-core/src/api
  ifneq ("$(wildcard $(TRYDIR)/m64p_types.h)","")
    CFLAGS += -I$(TRYDIR)
  else
    TRYDIR = /usr/local/include/mupen64plus
    ifneq ("$(wildcard $(TRYDIR)/m64p_types.h)","")
      CFLAGS += -I$(TRYDIR)
    else
      TRYDIR = /usr/include/mupen64plus
      ifneq ("$(wildcard $(TRYDIR)/m64p_types.h)","")
        CFLAGS += -I$(TRYDIR)
      endif
    endif
  endif
endif

# set special flags per-system
ifeq ($(OS), LINUX)
  # only export api symbols
  LDFLAGS += -Wl,-version-script,$(SRCDIR)/video_api_export.ver
endif
ifneq ($(OS), FREEBSD)
  ifeq ($(CPU), X86)
    # tweak flags for 32-bit build on 64-bit system
    ifeq ($(ARCH_DETECTED), 64BITS_32)
      CFLAGS += -m32
      LDFLAGS += -m32 -Wl,-m,elf_i386
    endif
  endif
else
  ifeq ($(ARCH_DETECTED), 64BITS_32)
    $(error Do not use the BITS=32 option with FreeBSD, use -m32 and -m elf_i386)
  endif
endif

# reduced compile output when running make without V=1
ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
	Q_CC  = @echo '    CC  '$@;
	Q_CXX = @echo '    CXX '$@;
	Q_LD  = @echo '    LD  '$@;
endif
endif

# set base program pointers and flags
CC       ?= gcc
CXX      ?= g++
RM       ?= rm -f
INSTALL  ?= install
MKDIR ?= mkdir -p
COMPILE.c = $(Q_CC)$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(Q_CXX)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK.o = $(Q_LD)$(CXX) $(LDFLAGS) $(TARGET_ARCH)

ifeq ($(OS),OSX)
  STRIP	?= strip -x 
else
  STRIP	?= strip -s
endif

# set special flags for given Makefile parameters
ifeq ($(DEBUG),1)
  CFLAGS += -g  -fno-inline -Werror
  STRIP = true # disable binary strip
endif

# set installation options
ifeq ($(PREFIX),)
  PREFIX := /usr/local
endif
ifeq ($(SHAREDIR),)
  SHAREDIR := $(PREFIX)/share/mupen64plus
endif
ifeq ($(LIBDIR),)
  LIBDIR := $(PREFIX)/lib
endif
ifeq ($(PLUGINDIR),)
  PLUGINDIR := $(LIBDIR)/mupen64plus
endif


SRCDIR = ../../src
OBJDIR = _obj

# list of source files to compile
SOURCE = \
	$(SRCDIR)/main.cpp \
	$(SRCDIR)/log/Logger.cpp \
	$(SRCDIR)/config/Config.cpp \
	$(SRCDIR)/config/StringFunctions.cpp \
	$(SRCDIR)/GraphicsPlugin.cpp \
	$(SRCDIR)/OpenGLManager.cpp \
	$(SRCDIR)/renderer/OpenGLRenderer.cpp \
	$(SRCDIR)/framebuffer/FrameBuffer.cpp \
	$(SRCDIR)/renderer/OpenGL2DRenderer.cpp \
	$(SRCDIR)/FogManager.cpp \
	$(SRCDIR)/MultiTexturingExt.cpp \
	$(SRCDIR)/ExtensionChecker.cpp \
	$(SRCDIR)/SecondaryColorExt.cpp \
	$(SRCDIR)/Memory.cpp \
	$(SRCDIR)/math/Matrix4.cpp \
	$(SRCDIR)/texture/CachedTexture.cpp \
	$(SRCDIR)/texture/TextureCache.cpp \
	$(SRCDIR)/texture/ImageFormatSelector.cpp \
	$(SRCDIR)/hash/CRCCalculator.cpp \
	$(SRCDIR)/hash/CRCCalculator2.cpp \
	$(SRCDIR)/texture/TextureLoader.cpp \
	$(SRCDIR)/DisplayListParser.cpp \
	$(SRCDIR)/VI.cpp \
	$(SRCDIR)/ucodes/UCodeSelector.cpp \
	$(SRCDIR)/ucodes/UCode0.cpp \
	$(SRCDIR)/ucodes/UCode1.cpp \
	$(SRCDIR)/ucodes/UCode2.cpp \
	$(SRCDIR)/ucodes/UCode3.cpp \
	$(SRCDIR)/ucodes/UCode4.cpp \
	$(SRCDIR)/ucodes/UCode5.cpp \
	$(SRCDIR)/ucodes/UCode6.cpp \
	$(SRCDIR)/ucodes/UCode7.cpp \
	$(SRCDIR)/ucodes/UCode8.cpp \
	$(SRCDIR)/ucodes/UCode9.cpp \
	$(SRCDIR)/ucodes/UCode10.cpp \
	$(SRCDIR)/GBI/GBI.cpp \
	$(SRCDIR)/RSP/RSP.cpp \
	$(SRCDIR)/RSP/RSPMatrixManager.cpp \
	$(SRCDIR)/RSP/RSPVertexManager.cpp \
	$(SRCDIR)/RSP/RSPLightManager.cpp \
	$(SRCDIR)/Combiner/AdvancedCombinerManager.cpp \
	$(SRCDIR)/Combiner/CombinerBase.cpp \
	$(SRCDIR)/Combiner/AdvancedTexEnvCombiner.cpp \
	$(SRCDIR)/Combiner/SimpleTexEnvCombiner.cpp \
	$(SRCDIR)/Combiner/DummyCombiner.cpp \
	$(SRCDIR)/Combiner/CombinerStageMerger.cpp \
	$(SRCDIR)/Combiner/CombinerStageCreator.cpp \
	$(SRCDIR)/Combiner/CombinerCache.cpp \
	$(SRCDIR)/RomDetector.cpp \
	$(SRCDIR)/RDP/RDP.cpp \
	$(SRCDIR)/RDP/RDPInstructions.cpp \
	$(SRCDIR)/osal_dynamiclib_unix.cpp


# generate a list of object files build, make a temporary directory for them
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(filter %.cpp, $(SOURCE)))
OBJDIRS = $(dir $(OBJECTS))
$(shell $(MKDIR) $(OBJDIRS))

# build targets

TARGET = mupen64plus-video-arachnoid.$(SO_EXTENSION)
targets:
	@echo "Mupen64plus-video-arachnoid N64 Graphics plugin makefile. "
	@echo "  Targets:"
	@echo "    all           == Build Mupen64plus-video-arachnoid plugin"
	@echo "    clean         == remove object files"
	@echo "    rebuild       == clean and re-build all"
	@echo "    install       == Install Mupen64Plus-video-arachnoid plugin"
	@echo "    uninstall     == Uninstall Mupen64Plus-video-arachnoid plugin"
	@echo "  Options:"
	@echo "    BITS=32       == build 32-bit binaries on 64-bit machine"
	@echo "    APIDIR=path   == path to find Mupen64Plus Core headers"
	@echo "    OPTFLAGS=flag == compiler optimization (default: -O3)"
	@echo "  Install Options:"
	@echo "    PREFIX=path   == install/uninstall prefix (default: /usr/local)"
	@echo "    LIBDIR=path   == library prefix (default: PREFIX/lib)"
	@echo "    PLUGINDIR=path == path to install plugin libraries (default: LIBDIR/mupen64plus)"
	@echo "    DESTDIR=path  == path to prepend to all installation paths (only for packagers)"
	@echo "  Debugging Options:"
	@echo "    DEBUG=1       == add debugging symbols"
	@echo "    V=1           == show verbose compiler output"

all: $(TARGET)

install: $(TARGET)
	$(INSTALL) -d "$(DESTDIR)$(PLUGINDIR)"
	$(INSTALL) -m 0644 $(TARGET) "$(DESTDIR)$(PLUGINDIR)"

uninstall:
	$(RM) "$(DESTDIR)$(PLUGINDIR)/$(TARGET)"


clean:
	$(RM) -r ./_obj mupen64plus-video-arachnoid.$(SO_EXTENSION)

# build dependency files
CFLAGS += -MD
-include $(OBJECTS:.o=.d)

CXXFLAGS += $(CFLAGS)

# standard build rules
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILE.c) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(COMPILE.cc) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
	$(STRIP) $@

.PHONY: all clean install uninstall targets
