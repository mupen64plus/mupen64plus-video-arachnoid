/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * http://bitbucket.org/wahrhaft/mupen64plus-video-arachnoid/
 *
 * Copyright (C) 2009 Jon Ring
 * Copyright (C) 2007 Kristofer Karlsson, Rickard Niklasson
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#ifndef WIN32
#include <cstring>
#include <byteswap.h>
#endif

// Swap bytes from 80 37 12 40
// to              40 12 37 80
// dwLen must be a multiple of 4
inline void swapRomHeaderBytes(void *v, unsigned int dwLen)
{
#ifdef WIN32
    __asm
    {
        mov        esi, v
        mov        edi, v
        mov        ecx, dwLen

        add        edi, ecx

top:
        mov        al, byte ptr [esi + 0]
        mov        bl, byte ptr [esi + 1]
        mov        cl, byte ptr [esi + 2]
        mov        dl, byte ptr [esi + 3]

        mov        byte ptr [esi + 0], dl        //3
        mov        byte ptr [esi + 1], cl        //2
        mov        byte ptr [esi + 2], bl        //1
        mov        byte ptr [esi + 3], al        //0

        add        esi, 4
        cmp        esi, edi
        jne        top
    }
#else
    int *b = (int*)v;
    dwLen /= 4;
    for (int i = 0; i < dwLen; ++i)
    {
        b[i] = bswap_32(b[i]);
    }
#endif
}


inline unsigned short swapword( unsigned short value )
{
#ifdef WIN32
    __asm
    {
        mov        ax, word ptr [value]
        xchg       ah, al
    }
#else
    return (value >> 8) | (value << 8);
#endif
}

inline void UnswapCopy( void *src, void *dest, unsigned int numBytes )
{
#ifdef WIN32
    __asm
    {
        mov        ecx, 0
        mov        esi, dword ptr [src]
        mov        edi, dword ptr [dest]

        mov        ebx, esi
        and        ebx, 3            // ebx = number of leading bytes

        cmp        ebx, 0
         jz        StartDWordLoop
        neg        ebx
        add        ebx, 4

        cmp        ebx, [numBytes]
        jle        NotGreater
        mov        ebx, [numBytes]
NotGreater:
        mov        ecx, ebx
        xor        esi, 3
LeadingLoop:                // Copies leading bytes, in reverse order (un-swaps)
        mov        al, byte ptr [esi]
        mov        byte ptr [edi], al
        sub        esi, 1
        add        edi, 1
        loop       LeadingLoop
        add        esi, 5

StartDWordLoop:
        mov        ecx, dword ptr [numBytes]
        sub        ecx, ebx        // Don't copy what's already been copied

        mov        ebx, ecx
        and        ebx, 3
//        add        ecx, 3            // Round up to nearest dword
        shr        ecx, 2

        cmp        ecx, 0            // If there's nothing to do, don't do it
        jle        StartTrailingLoop

        // Copies from source to destination, bswap-ing first
DWordLoop:
        mov        eax, dword ptr [esi]
        bswap      eax
        mov        dword ptr [edi], eax
        add        esi, 4
        add        edi, 4
        loop    DWordLoop
StartTrailingLoop:
        cmp        ebx, 0
        jz         Done
        mov        ecx, ebx
        xor        esi, 3

TrailingLoop:
        mov        al, byte ptr [esi]
        mov        byte ptr [edi], al
        sub        esi, 1
        add        edi, 1
        loop       TrailingLoop
Done:
    }
#else

    long beginOffset = (long)src & 3;
    char *readPtr = (char*)src - beginOffset;
    char *writePtr = (char*)dest;
    
    int swapOffset = beginOffset;
    for (int i = 0; i < numBytes; ++i)
    {
        *writePtr = readPtr[3 - swapOffset];
        ++writePtr;
        ++swapOffset;
        if (swapOffset > 3)
        {
            swapOffset = 0;
            readPtr += 4;
        }
    }

#endif
}

inline void DWordInterleave( void *mem, unsigned int numDWords )
{
#ifdef WIN32
    __asm {
        mov        esi, dword ptr [mem]
        mov        edi, dword ptr [mem]
        add        edi, 4
        mov        ecx, dword ptr [numDWords]
DWordInterleaveLoop:
        mov        eax, dword ptr [esi]
        mov        ebx, dword ptr [edi]
        mov        dword ptr [esi], ebx
        mov        dword ptr [edi], eax
        add        esi, 8
        add        edi, 8
        loop       DWordInterleaveLoop
    }
#else
    int *m = (int*)mem;
    for (unsigned int i = 0; i < numDWords; ++i)
    {
        int tmp = m[2 * i];
        m[2 * i] = m[2 * i + 1];
        m[2 * i + 1] = tmp;
    }
#endif
}

inline void QWordInterleave( void *mem, unsigned int numDWords )
{
#ifdef WIN32
    __asm
    {
    // Interleave the line on the qword
        mov        esi, dword ptr [mem]
        mov        edi, dword ptr [mem]
        add        edi, 8
        mov        ecx, dword ptr [numDWords]
        shr        ecx, 1
QWordInterleaveLoop:
        mov        eax, dword ptr [esi]
        mov        ebx, dword ptr [edi]
        mov        dword ptr [esi], ebx
        mov        dword ptr [edi], eax
        add        esi, 4
        add        edi, 4
        mov        eax, dword ptr [esi]
        mov        ebx, dword ptr [edi]
        mov        dword ptr [esi], ebx
        mov        dword ptr [edi], eax
        add        esi, 12
        add        edi, 12
        loop       QWordInterleaveLoop
    }
#else
    long long *m = (long long*)mem;
    for (unsigned int i = 0; i < numDWords / 2; ++i)
    {
        long long tmp = m[2 * i];
        m[2 * i] = m[2 * i + 1];
        m[2 * i + 1] = tmp;
    }
#endif
}

const unsigned char Five2Eight[32] =
{
      0, // 00000 = 00000000
      8, // 00001 = 00001000
     16, // 00010 = 00010000
     25, // 00011 = 00011001
     33, // 00100 = 00100001
     41, // 00101 = 00101001
     49, // 00110 = 00110001
     58, // 00111 = 00111010
     66, // 01000 = 01000010
     74, // 01001 = 01001010
     82, // 01010 = 01010010
     90, // 01011 = 01011010
     99, // 01100 = 01100011
    107, // 01101 = 01101011
    115, // 01110 = 01110011
    123, // 01111 = 01111011
    132, // 10000 = 10000100
    140, // 10001 = 10001100
    148, // 10010 = 10010100
    156, // 10011 = 10011100
    165, // 10100 = 10100101
    173, // 10101 = 10101101
    181, // 10110 = 10110101
    189, // 10111 = 10111101
    197, // 11000 = 11000101
    206, // 11001 = 11001110
    214, // 11010 = 11010110
    222, // 11011 = 11011110
    230, // 11100 = 11100110
    239, // 11101 = 11101111
    247, // 11110 = 11110111
    255  // 11111 = 11111111
};

const unsigned char Four2Eight[16] = 
{
      0, // 0000 = 00000000
     17, // 0001 = 00010001
     34, // 0010 = 00100010
     51, // 0011 = 00110011
     68, // 0100 = 01000100
     85, // 0101 = 01010101
    102, // 0110 = 01100110
    119, // 0111 = 01110111
    136, // 1000 = 10001000
    153, // 1001 = 10011001
    170, // 1010 = 10101010
    187, // 1011 = 10111011
    204, // 1100 = 11001100
    221, // 1101 = 11011101
    238, // 1110 = 11101110
    255  // 1111 = 11111111
};

const unsigned char Three2Four[8] = 
{
     0, // 000 = 0000
     2, // 001 = 0010
     4, // 010 = 0100
     6, // 011 = 0110
     9, // 100 = 1001
    11, // 101 = 1011
    13, // 110 = 1101
    15, // 111 = 1111
};

const unsigned char Three2Eight[8] = 
{
      0, // 000 = 00000000
     36, // 001 = 00100100
     73, // 010 = 01001001
    109, // 011 = 01101101
    146, // 100 = 10010010
    182, // 101 = 10110110
    219, // 110 = 11011011
    255, // 111 = 11111111
};
const unsigned char Two2Eight[4] = 
{
      0, // 00 = 00000000
     85, // 01 = 01010101
    170, // 10 = 10101010
    255  // 11 = 11111111
};

const unsigned char One2Four[2] = 
{
     0, // 0 = 0000
    15, // 1 = 1111
};

const unsigned char One2Eight[2] = 
{
      0, // 0 = 00000000
    255, // 1 = 11111111
};

inline unsigned short RGBA8888_RGBA4444( unsigned int color )
{
#ifdef WIN32
    __asm
    {
        mov        ebx, dword ptr [color]
        // R
        and        bl, 0F0h
        mov        ah, bl

        // G
        shr        bh, 4
        or         ah, bh

        bswap      ebx

        // B
        and        bh, 0F0h
        mov        al, bh

        // A
        shr        bl, 4
        or         al, bl
    }
#else
    return ((color & 0xF0000000) >> 28) | ((color & 0x00F00000) >> 16) |
           ((color & 0x0000F000) >>  4) | ((color & 0x000000F0) << 8);
#endif
}

inline unsigned int RGBA5551_RGBA8888( unsigned short color )
{
#ifdef WIN32
    __asm
    {
        mov        ebx, 00000000h
        mov        cx, word ptr [color]
        xchg       cl, ch

        mov        bx, cx
        and        bx, 01h
        mov        al, byte ptr [One2Eight+ebx]

        mov        bx, cx
        shr        bx, 01h
        and        bx, 1Fh
        mov        ah, byte ptr [Five2Eight+ebx]

        bswap      eax

        mov        bx, cx
        shr        bx, 06h
        and        bx, 1Fh
        mov        ah, byte ptr [Five2Eight+ebx]

        mov        bx, cx
        shr        bx, 0Bh
        and        bx, 1Fh
        mov        al, byte ptr [Five2Eight+ebx]
    }
#else
    int rgba;
    char *p = (char*)&rgba;
    color = (color >> 8) | (color << 8);
    p[3] = One2Eight[color & 1];
    p[2] = Five2Eight[color >> 1 & 0x1f];
    p[1] = Five2Eight[color >> 6 & 0x1f];
    p[0] = Five2Eight[color >> 11 & 0x1f];
    return rgba;

#endif
}

// Just swaps the word
inline unsigned short RGBA5551_RGBA5551( unsigned short color )
{
#ifdef WIN32
    __asm
    {
        mov        ax, word ptr [color]
        xchg       ah, al
    }
#else
    return (color >> 8) | (color << 8);
#endif
}

inline unsigned int IA88_RGBA8888( unsigned short color )
{
#ifdef WIN32
    __asm
    {
        mov        cx, word ptr [color]

        mov        al, ch
        mov        ah, cl

        bswap      eax

        mov        ah, cl
        mov        al, cl
    }
#else
    return (color & 0xFF) | ((color & 0xFF) << 8) | (color << 16);
#endif
}

inline unsigned short IA88_RGBA4444( unsigned short color )
{
#ifdef WIN32
    __asm
    {
        mov        cx, word ptr [color]

        shr        cl, 4
        mov        ah, cl
        shl        cl, 4
        or         ah, cl
        mov        al, cl

        shr        ch, 4
        or         al, ch
    }
#else
    unsigned char b = color & 0xf0;
    return (color >> 12) | b | (b << 4) | (b << 8);
#endif
}

inline unsigned short IA44_RGBA4444( unsigned char color )
{
#ifdef WIN32
    __asm
    {
        mov        cl, byte ptr [color]
        mov        al, cl

        shr        cl, 4
        mov        ah, cl
        shl        cl, 4
        or         ah, cl
    }
#else
    unsigned char b = color >> 4;
    return color | (b << 8) | (b << 12);
#endif
}

inline unsigned int IA44_RGBA8888( unsigned char color )
{
#ifdef WIN32
    __asm
    {
        mov        ebx, 00000000h
        mov        cl, byte ptr [color]

        mov        bl, cl
        shr        bl, 04h
        mov        ch, byte ptr [Four2Eight+ebx]

        mov        bl, cl
        and        bl, 0Fh
        mov        cl, byte ptr [Four2Eight+ebx]

        mov        al, cl
        mov        ah, ch

        bswap      eax

        mov        ah, ch
        mov        al, ch
    }
#else
    unsigned char b1 = color >> 4;
    unsigned char b2 = color & 0x0f;
    return b1 | (b1 << 4) | (b1 << 8) | (b1 << 12) | (b1 << 16) | (b1 << 20) | (b2 << 24) | (b2 << 28);
#endif
}

inline unsigned short IA31_RGBA4444( unsigned char color )
{
#ifdef WIN32
    __asm
    {
        mov        ebx, 00000000h
        mov        cl, byte ptr [color]

        mov        bl, cl
        shr        bl, 01h
        mov        ch, byte ptr [Three2Four+ebx]
        mov        ah, ch
        shl        ch, 4
        or         ah, ch
        mov        al, ch

        mov        bl, cl
        and        bl, 01h
        mov        ch, byte ptr [One2Four+ebx]
        or         al, ch
    }
#else
    unsigned char t = Three2Four[color >> 1];
    return One2Four[color & 1] | (t << 4) | (t << 8) | (t << 12); 
#endif
}

inline unsigned int IA31_RGBA8888( unsigned char color )
{
#ifdef WIN32
    __asm
    {
        mov        ebx, 00000000h
        mov        cl, byte ptr [color]

        mov        bl, cl
        shr        bl, 01h
        mov        ch, byte ptr [Three2Eight+ebx]

        mov        bl, cl
        and        bl, 01h
        mov        cl, byte ptr [One2Eight+ebx]

        mov        al, cl
        mov        ah, ch

        bswap      eax

        mov        ah, ch
        mov        al, ch
    }
#else
    unsigned char t = Three2Eight[color >> 1];
    return t | (t << 8) | (t << 16) | (One2Eight[color & 1] << 24);
#endif
}

inline unsigned short I8_RGBA4444( unsigned char color )
{
#ifdef WIN32
    __asm
    {
        mov        cl, byte ptr [color]

        shr        cl, 4
        mov        al, cl
        shl        cl, 4
        or         al, cl
        mov        ah, al
    }
#else
    color &= 0xf0;
    return (color >> 4) | color | (color << 4) | (color << 8);
#endif
}

inline unsigned int I8_RGBA8888( unsigned char color )
{
#ifdef WIN32
    __asm
    {
        mov        cl, byte ptr [color]

        mov        al, cl
        mov        ah, cl
        bswap      eax
        mov        ah, cl
        mov        al, cl
    }
#else
    return color | (color << 8) | (color << 16) | (color << 24);
#endif
}

inline unsigned short I4_RGBA4444( unsigned char color )
{
#ifdef WIN32
    __asm
    {
        mov        cl, byte ptr [color]
        mov        al, cl
        shl        cl, 4
        or         al, cl
        mov        ah, al
    }
#else
    color &= 0x0f;
    return color | (color << 4) | (color << 8) | (color << 12);
#endif
}

inline unsigned int I4_RGBA8888( unsigned char color )
{
#ifdef WIN32
    __asm
    {
        mov        ebx, 00000000h

        mov        bl, byte ptr [color]
        mov        cl, byte ptr [Four2Eight+ebx]

        mov        al, cl
        mov        ah, cl
        bswap      eax
        mov        ah, cl
        mov        al, cl
    }
#else
    unsigned char b = Four2Eight[color];
    return b | (b << 8) | (b << 16) | (b << 24);
#endif
}

#endif
