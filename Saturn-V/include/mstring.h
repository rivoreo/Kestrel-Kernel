/*	Multi String Library
	Copyright 2007-2014 PC GO Ld.

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef _MULTI_STRING_H
#define _MULTI_STRING_H

#include <kestrel/types.h>

typedef char *mstr;
typedef mstr MultiString;

size_t mstrsize(mstr);
mstr mstradd(mstr, const char *);
mstr mstradd_r(mstr *, const char *);
mstr mstrinit(const char *, size_t);
int mstrcount(mstr);
char *mstrindex(mstr, unsigned int);
mstr mstrcpy(mstr, mstr);
mstr mstrdup(mstr);

int putmss(mstr, unsigned int);
int putms(mstr);

mstr mkmstrl(const char *, ...);
mstr mkmstrv(char **);

void *malloc(size_t);
void *realloc(void *, size_t);
void free(void *);

#endif
