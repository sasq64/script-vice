/*
 * platform_x86_gcc_cpuid.h - x86 gcc cpuid code.
 *
 * Written by
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#ifndef PLATFORM_X86_GCC_CPUID_H
#define PLATFORM_X86_GCC_CPUID_H

#if defined(__PIC__) && !defined(OpenBSD5_7) && !defined(__CYGWIN32__) && !defined(__CYGWIN__) && !defined(WIN32_COMPILE)

#define cpuid(func, ax, bx, cx, dx) \
__asm__ __volatile__("pushq %%rbx     \n\t" \
                     "cpuid            \n\t" \
                     "movl %%ebx, %1   \n\t" /* save what cpuid just put in %ebx */ \
                     "popq %%rbx \n\t" /* restore the old %ebx */ \
                     : "=a"(ax), "=r"(bx), "=c"(cx), "=d"(dx) \
                     : "a"(func) \
                     : "cc");

#else
#define cpuid(func, ax, bx, cx, dx) \
    __asm__ __volatile__ (".byte 15;.byte 162":  \
    "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func): "cc")
#endif

inline static int has_cpuid(void)
{
#if defined(__amd64__) || defined(__x86_64__)
    return 1;
#else
    int a = 0;
    int c = 0;

    __asm__ __volatile__ ("pushf;"
                          "popl %0;"
                          "movl %0, %1;"
                          "xorl $0x200000, %0;"
                          "push %0;"
                          "popf;"
                          "pushf;"
                          "popl %0;"
                          : "=a" (a), "=c" (c)
                          :
                          : "cc" );
    return (a != c);
#endif
}

#endif
