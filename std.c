 
#include <stdio.h>
#include <stdlib.h>
#include "stdarg.h"
#include <string.h>
#include <ctype.h>

#define ASSERT(value) if (!(value)) {   __asm__ __volatile__("hlt"); /*_asm {int 3};*/}

void checkstring(char *s) {
	ASSERT( s != NULL);
}

#define  __func__ __FUNCTION__

#define bool  int
#define true  1
#define false 0

void LOG(char *msg) {
	printf("%s\n", msg);
}

void DBLOG(char *patern, ...) {
	char buf[512];
	va_list vlist;
	va_start(vlist, patern);
	vsnprintf(buf, 512, patern, vlist);
	va_end(vlist);
	LOG(buf);
}

void ERR(char *msg) {
	LOG(msg);
	ASSERT(0);
}

void DBERR(char *patern, ...) {
	char buf[512];
	va_list vlist;
	va_start(vlist, patern);
	vsnprintf(buf, 512, patern, vlist);
	va_end(vlist);
	LOG(buf);
	ASSERT(0);
}

long sizef(FILE *pf) {
	long cur, siz;
	cur = ftell(pf);
	fseek(pf, 0, SEEK_END);
	siz = ftell(pf);
	fseek(pf, cur, SEEK_SET);
	return siz;
}

char *data(char *fname, long *out_siz_buf) {
	FILE *pf;
	char *buf = NULL;
	long siz, siz_r=0;
	if ( (pf = fopen(fname,"rb")) != 0) {
		siz = sizef(pf);
		buf = (char*)malloc(siz); if (buf == 0) { ERR("malloc fail. in data()"); }
		siz_r = fread(buf, 1, siz, pf); if (siz_r != siz) { ERR("Error in data(). fread file siz not correct"); }
	} else {
		DBERR("can't open file '%s' in %s", fname, "data");
	}
	*out_siz_buf = siz_r;
	fclose(pf);
	return buf;
}

void dataWrite(char *fname, char *dat, int siz) {
	FILE *pf;
	if ( (pf = fopen(fname,"wb")) != 0) {
        fwrite(dat,siz,1,pf);
	} else {
		DBERR("can't open file '%s' in %s", fname, "dataWrite");
	}
	fclose(pf);
}

// buf 指向的数组大小增加n 字节，新增的字节置初值0。警告：该函数可能会改变buf 指向的内存地址，使用时请千万小心。
void increases_nbytes(char **buf, int *siz, int n) {
	char * r;
	if (*buf == NULL && *siz != 0) DBERR("Error: bad argument. in fuction %s()", __func__);
	if (*siz < 0 || n <= 0) DBERR("Error: bad argument. in fuction %s()", __func__);
	r = realloc(*buf, *siz + n);
	if (r == NULL) DBERR("ERROR: realloc fail. in function %s()", __func__);
	if (r != *buf) LOG("Warning: realloc change point's addr");
	*buf = r;
	memset(*buf+*siz, 0, n);
	*siz = *siz + n;
}

// use for free memory that allocate by split() function
void free_split_memory(unsigned int *pointerArray) {
	if (pointerArray != NULL) {
		free( (void*)(pointerArray[0]) );  // free memory that pointerArray pointer to
		free( (void*)pointerArray );       // free pointerArray itself
	}
}

// @buf     : a string that terminal with NULL, it contains more word, each word use  a seperater [3th argument c] splits.
// @siz     : nbyte
// @c       : seperater
// @out_siz : nbyte of the buffer that this function return. it was a pointer array exactly.
unsigned int *split(const char *buf, int siz, char c, int *out_siz) {
	const char de[2] = {c, 0};
	char *bf;
	unsigned int *pointerArray = NULL; int szofpointerArray = 0; int nPointer = szofpointerArray / (sizeof (char*));
	int i = 0;
	if (buf == NULL || siz <= 0 /*|| siz > 1024*/ || buf[siz -1] != 0 || strlen(buf) != siz - 1) DBERR("ERROR: bad argument. in fuction %s()", __func__);
	bf = (char*)calloc(1, siz);
    
	memcpy(bf, buf, siz);
	increases_nbytes( (char**)&pointerArray, &szofpointerArray, sizeof(char*) );
	pointerArray[i] = (unsigned int)strtok(bf, de);
	if (pointerArray[i] != 0) {
		//DBLOG("%s", (char*)(*pointerArray));
		char *p;
		while ( (p=strtok(NULL, de)) != NULL ) {
			increases_nbytes( (char**)&pointerArray, &szofpointerArray, sizeof(char*) );
			i = i + 1;
			pointerArray[i] = (unsigned int)p;
			//DBLOG("%s", (char*)(*(pointerArray+i)) );
		}
	} else {
		free((void*)pointerArray);
		pointerArray = NULL;
		return NULL;
	}
	nPointer = szofpointerArray / (sizeof (char*));
	//DBLOG("nPointer = %d", nPointer);
	//*out_siz = szofpointerArray;
	*out_siz = nPointer;
	return pointerArray;
}
