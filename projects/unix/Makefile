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
ifeq ("$(UNAME)","Darwin")
  OS = OSX
  LDFLAGS += -liconv -lpng
  SO_EXTENSION = dylib
  SHARED = -bundle
endif
ifeq ("$(UNAME)","FreeBSD")
  OS = FREEBSD
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

# base CFLAGS, LIBS, and LDFLAGS
CFLAGS = -ffast-math -funroll-loops -fexpensive-optimizations -fno-strict-aliasing -I../../src \
		 -I../../src/hash -I../../src/ucodes -I../../src/GBI -I../../src/RDP -I../../src/utils \
		 -I../../src/log -I../../src/RSP -I../../src/framebuffer -I../../src/math -I../../src/renderer \
		 -I../../src/Assembler -I../../src/texture -I../../src/config -I../../src/Combiner 
ifneq ($(OS), FREEBSD)
  CFLAGS += -pipe -O3
endif

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
  # flags for other Unices
  LDFLAGS += -ldl -lGL -lGLU -lpthread
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
ifneq ($(OS), FREEBSD)
  ifeq ($(CPU), X86)
    ifeq ($(ARCH_DETECTED), 64BITS)
      CFLAGS += -march=athlon64
    else
      CFLAGS += -mmmx -msse -march=i686 -mtune=pentium-m -fomit-frame-pointer
    endif
    # tweak flags for 32-bit build on 64-bit system
    ifeq ($(ARCH_DETECTED), 64BITS_32)
      CFLAGS += -m32
      LDFLAGS += -m32 -m elf_i386
    endif
  endif
else
  ifeq ($(ARCH_DETECTED), 64BITS_32)
    $(error Do not use the BITS=32 option with FreeBSD, use -m32 and -m elf_i386)
  endif
endif
ifeq ($(CPU), PPC)
  CFLAGS += -mcpu=powerpc
endif

# set shell function names
# set shell function names
CC      ?= gcc
CXX     ?= g++
LD      ?= g++
INSTALL ?= install
ifeq ($(OS),LINUX)
  STRIP	= strip -s
endif
ifeq ($(OS),OSX)
  STRIP	= strip -x 
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
  LIBDIR := $(PREFIX)/lib/mupen64plus
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
$(shell mkdir -p $(OBJDIRS))

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
	@echo "  Install Options:"
	@echo "    PREFIX=path   == install/uninstall prefix (default: /usr/local)"
	@echo "    LIBDIR=path   == path to install plugin libraries (default: PREFIX/lib/mupen64plus)"
	@echo "    DESTDIR=path  == path to prepend to all installation paths (only for packagers)"
	@echo "  Debugging Options:"
	@echo "    DEBUG=1       == add debugging symbols"

all: $(TARGET)

install: $(TARGET)
	$(INSTALL) -d -v "$(DESTDIR)$(LIBDIR)"
	$(INSTALL) -m 0644 $(TARGET) "$(DESTDIR)$(LIBDIR)"

uninstall:
	rm -f "$(DESTDIR)$(LIBDIR)/$(TARGET)"


clean:
	rm -rf ./_obj mupen64plus-video-arachnoid.$(SO_EXTENSION)

# build rules

$(TARGET): $(OBJECTS)
	$(CXX) $(SHARED) $^ $(LDFLAGS) -o $@
	$(STRIP) $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ $(CFLAGS) -c $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -o $@ $(CFLAGS) -c $<

