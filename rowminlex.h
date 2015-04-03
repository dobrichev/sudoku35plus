//Code inspired by Glenn Fowler's sudoku solver/generator
//The original is published under the following license
		/***********************************************************************
		*               This software is part of the ast package               *
		*          Copyright (c) 2005-2009 AT&T Intellectual Property          *
		*                      and is licensed under the                       *
		*                  Common Public License, Version 1.0                  *
		*                    by AT&T Intellectual Property                     *
		*                                                                      *
		*                A copy of the License is available at                 *
		*            http://www.opensource.org/licenses/cpl1.0.txt             *
		*         (with md5 checksum 059e8cd6165cb4c31e351f2b69388fd9)         *
		*                                                                      *
		*              Information and Software Systems Research               *
		*                            AT&T Research                             *
		*                           Florham Park NJ                            *
		*                                                                      *
		*                 Glenn Fowler <gsf@research.att.com>                  *
		***********************************************************************/
//This is a MODIFIED version of the original code

#ifndef ROWMINLEX_H_INCLUDED

#define ROWMINLEX_H_INCLUDED

#include <memory.h>

#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER) || defined(__INTEL_COMPILER)
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE
#endif

/* Define answer structure */
typedef struct Answer_s {
  unsigned short type;
  short sord[3];
  short rmap1[9], cmap1[9], smap1[10];
  short rmap2[9], cmap2[9], smap2[10];
} Answer_t;

struct subCanoner {
	Answer_t *Oldstk;
	subCanoner();
	~subCanoner();
	void canon(const char* restrict puz, char* restrict can) restrict;
	//static void subcanon(const char* restrict puz, char* restrict can);
};

extern void subcanon(const char* restrict puz, char* restrict can);

#endif // ROWMINLEX_H_INCLUDED
