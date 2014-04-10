//
//  std.h
//  array
//
//  Created by appleUser on 4/3/14.
//  Copyright (c) 2014 Big Nerd Ranch. All rights reserved.
//

#ifndef _std_h
#define _std_h

#define ASSERT(value) if (!(value)) {   __asm__ __volatile__("hlt"); /*_asm {int 3};*/}
void LOG(char *msg);
void DBLOG(char *patern, ...);
void ERR(char *msg);
void DBERR(char *patern, ...);
long sizef(FILE *pf);
char *data(char *fname, int *out_siz_buf);
void *dataWrite(char *fname, char *dat, int siz);
void increases_nbytes(char **buf, int *siz, int n);
void free_split_memory(unsigned int *pointerArray);
unsigned int *split(const char *buf, int siz, char c, int *out_siz);

#endif
