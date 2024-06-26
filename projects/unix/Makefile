#/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# *   Arachnoid - Makefile                                                  *
# *   https://github.com/mupen64plus/mupen64plus-video-arachnoid/           *
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

# detect operating system
UNAME ?= $(shell uname -s)
OS := NONE
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
endif
ifneq ("$(filter GNU/kFreeBSD kfreebsd,$(UNAME))","")
  OS = LINUX
  SO_EXTENSION = so
  SHARED = -shared
endif
ifeq ("$(patsubst MINGW%,MINGW,$(UNAME))","MINGW")
  OS = MINGW
  SO_EXTENSION = dll
  SHARED = -shared
  PIC = 0
endif
ifeq ("$(OS)","NONE")
  $(error OS type "$(UNAME)" not supported.  Please file bug report at 'https://github.com/mupen64plus/mupen64plus-core/issues')
endif

# detect system architecture
HOST_CPU ?= $(shell uname -m)
CPU := NONE
NO_ASM ?= 1
ifneq ("$(filter x86_64 amd64,$(HOST_CPU))","")
  CPU := X86
  ifeq ("$(BITS)", "32")
    ARCH_DETECTED := 64BITS_32
    PIC ?= 0
  else
    ARCH_DETECTED := 64BITS
    PIC ?= 1
  endif
endif
ifneq ("$(filter pentium i%86,$(HOST_CPU))","")
  CPU := X86
  ARCH_DETECTED := 32BITS
  PIC ?= 0
endif
ifneq ("$(filter ppc macppc socppc powerpc,$(HOST_CPU))","")
  CPU := PPC
  ARCH_DETECTED := 32BITS
  BIG_ENDIAN := 1
  PIC ?= 1
  $(warning Architecture "$(HOST_CPU)" not officially supported.')
endif
ifneq ("$(filter ppc64 powerpc64,$(HOST_CPU))","")
  CPU := PPC
  ARCH_DETECTED := 64BITS
  BIG_ENDIAN := 1
  PIC ?= 1
  $(warning Architecture "$(HOST_CPU)" not officially supported.')
endif
ifneq ("$(filter arm%,$(HOST_CPU))","")
  ifeq ("$(filter arm%b,$(HOST_CPU))","")
    CPU := ARM
    ARCH_DETECTED := 32BITS
    PIC ?= 1
    $(warning Architecture "$(HOST_CPU)" not officially supported.')
  endif
endif
ifeq ("$(CPU)","NONE")
  $(error CPU type "$(HOST_CPU)" not supported.  Please file bug report at 'https://github.com/mupen64plus/mupen64plus-core/issues')
endif

SRCDIR = ../../src
OBJDIR = _obj$(POSTFIX)

# base CFLAGS, LDLIBS, and LDFLAGS
OPTFLAGS ?= -O3 -flto
WARNFLAGS ?= -Wall
CFLAGS += $(OPTFLAGS) $(WARNFLAGS) -ffast-math -fno-strict-aliasing -fvisibility=hidden -I../../src \
		 -I../../src/hash -I../../src/ucodes -I../../src/GBI -I../../src/RDP -I../../src/utils \
		 -I../../src/log -I../../src/RSP -I../../src/framebuffer -I../../src/math -I../../src/renderer \
		 -I../../src/Assembler -I../../src/texture -I../../src/config -I../../src/Combiner 
CXXFLAGS += -fvisibility-inlines-hidden
LDFLAGS += $(SHARED)

# On OS X, add a few extra flags to elegantly support cross-compilation and backward
# compatibility (and also the flags to link against OpenGL)
ifeq ($(OS), OSX)
  # Select the proper SDK
  # Also, SDKs are stored in a different location since XCode 4.3
  OSX_SDK ?= $(shell sw_vers -productVersion | cut -f1 -f2 -d .)
  OSX_XCODEMAJ = $(shell xcodebuild -version | grep '[0-9]*\.[0-9]*' | cut -f2 -d ' ' | cut -f1 -d .)
  OSX_XCODEMIN = $(shell xcodebuild -version | grep '[0-9]*\.[0-9]*' | cut -f2 -d ' ' | cut -f2 -d .)
  OSX_XCODEGE43 = $(shell echo "`expr $(OSX_XCODEMAJ) \>= 4``expr $(OSX_XCODEMIN) \>= 3`")
  ifeq ($(OSX_XCODEGE43), 11)
    OSX_SYSROOT := /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs
  else
    OSX_SYSROOT := /Developer/SDKs
  endif

  ifeq ($(CPU), X86)
    ifeq ($(ARCH_DETECTED), 64BITS)
      CFLAGS += -arch x86_64 -mmacosx-version-min=$(OSX_SDK) -isysroot $(OSX_SYSROOT)/MacOSX$(OSX_SDK).sdk
      LDFLAGS += -bundle
    else
      CFLAGS += -arch i686 -mmacosx-version-min=$(OSX_SDK) -isysroot $(OSX_SYSROOT)/MacOSX$(OSX_SDK).sdk
      LDFLAGS += -bundle
    endif
  endif
else
  # search for OpenGL libraries
  ifeq ($(OS), OSX)
    GL_LDLIBS = -framework OpenGL
  endif
  ifeq ($(OS), MINGW)
    GL_LDLIBS = -lopengl32
  endif

  ifeq ($(origin GL_CFLAGS) $(origin GL_LDLIBS), undefined undefined)
    ifeq ($(origin PKG_CONFIG), undefined)
      PKG_CONFIG = $(CROSS_COMPILE)pkg-config
      ifeq ($(shell which $(PKG_CONFIG) 2>/dev/null),)
        $(error $(PKG_CONFIG) not found)
      endif
    endif

    ifeq ($(shell $(PKG_CONFIG) --modversion gl 2>/dev/null),)
      $(error No OpenGL development libraries found!)
    endif
    GL_CFLAGS += $(shell $(PKG_CONFIG) --cflags gl)
    GL_LDLIBS +=  $(shell $(PKG_CONFIG) --libs gl)
  endif
  CFLAGS += $(GL_CFLAGS)
  LDLIBS += $(GL_LDLIBS)

  CFLAGS += -pthread
endif
ifeq ($(OS), LINUX)
  LDLIBS += -ldl 
endif
ifeq ($(OS), FREEBSD)
  LDLIBS += -lc
endif


# Since we are building a shared library, we must compile with -fPIC on some architectures
# On 32-bit x86 systems we do not want to use -fPIC because we don't have to and it has a big performance penalty on this arch
ifeq ($(PIC), 1)
  CFLAGS += -fPIC
else
  CFLAGS += -fno-PIC
endif

ifeq ($(BIG_ENDIAN), 1)
  CFLAGS += -DM64P_BIG_ENDIAN
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

# tweak flags for 32-bit build on 64-bit system
ifeq ($(ARCH_DETECTED), 64BITS_32)
  ifeq ($(OS), FREEBSD)
    $(error Do not use the BITS=32 option with FreeBSD, use -m32 and -m elf_i386)
  endif
  ifneq ($(OS), OSX)
    ifeq ($(OS), MINGW)
      LDFLAGS += -Wl,-m,i386pe
    else
      CFLAGS += -m32
      LDFLAGS += -Wl,-m,elf_i386
    endif
  endif
endif

ifeq ($(ARCH_DETECTED), 64BITS)
  ifeq ($(OS), MINGW)
    LDFLAGS += -Wl,-m,i386pep
  endif
endif

# set special flags per-system
ifeq ($(OS), LINUX)
  # only export api symbols
  LDFLAGS += -Wl,-version-script,$(SRCDIR)/video_api_export.ver
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
CC        = $(CROSS_COMPILE)gcc
CXX       = $(CROSS_COMPILE)g++
RM       ?= rm -f
INSTALL  ?= install
MKDIR ?= mkdir -p
COMPILE.c = $(Q_CC)$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(Q_CXX)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK.o = $(Q_LD)$(CXX) $(CXXFLAGS) $(LDFLAGS) $(TARGET_ARCH)

# set special flags for given Makefile parameters
ifeq ($(DEBUG),1)
  CFLAGS += -g
  INSTALL_STRIP_FLAG ?= 
else
  INSTALL_STRIP_FLAG ?= -s
endif

# set installation options
ifeq ($(PREFIX),)
  PREFIX := /usr/local
endif
ifeq ($(LIBDIR),)
  LIBDIR := $(PREFIX)/lib
endif
ifeq ($(PLUGINDIR),)
  PLUGINDIR := $(LIBDIR)/mupen64plus
endif

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
	$(SRCDIR)/RDP/RDPInstructions.cpp

ifeq ($(OS),MINGW)
SOURCE += $(SRCDIR)/osal_dynamiclib_win32.cpp
else
SOURCE += $(SRCDIR)/osal_dynamiclib_unix.cpp
endif


# generate a list of object files build, make a temporary directory for them
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(filter %.cpp, $(SOURCE)))
OBJDIRS = $(dir $(OBJECTS))
$(shell $(MKDIR) $(OBJDIRS))

# build targets

TARGET = mupen64plus-video-arachnoid$(POSTFIX).$(SO_EXTENSION)
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
	@echo "    OPTFLAGS=flag == compiler optimization (default: -O3 -flto)"
	@echo "    WARNFLAGS=flag == compiler warning levels (default: -Wall)"
	@echo "    PIC=(1|0)     == Force enable/disable of position independent code"
	@echo "    POSTFIX=name  == String added to the name of the the build (default: '')"
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
	$(INSTALL) -m 0644 $(INSTALL_STRIP_FLAG) $(TARGET) "$(DESTDIR)$(PLUGINDIR)"

uninstall:
	$(RM) "$(DESTDIR)$(PLUGINDIR)/$(TARGET)"


clean:
	$(RM) -r $(OBJDIR) $(TARGET)

# build dependency files
CFLAGS += -MD -MP
-include $(OBJECTS:.o=.d)

CXXFLAGS += $(CFLAGS)

# standard build rules
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILE.c) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(COMPILE.cc) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: all clean install uninstall targets
