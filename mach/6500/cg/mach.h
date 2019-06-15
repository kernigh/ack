/*
 * (c) copyright 1987 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Copyright".
 */
/* $Id$ */

#include "tables.h"
#include "types.h"

#define ex_ap(y)        fprintf(codefile,".extern %s\n",y)
#define in_ap(y)        /* nothing */

#define newilb(x)       fprintf(codefile,"%s:\n",x)
#define newdlb(x)       fprintf(codefile,"%s:\n",x)
#define dlbdlb(x,y)     fprintf(codefile,"%s = %s\n",x,y)
#define newlbss(l,x)    fprintf(codefile,".comm %s,%u\n",l,x);

#define cst_fmt         "%d"
#define off_fmt         "%d"
#define ilb_fmt         "I%x_%x"
#define dlb_fmt         "_%d"
#define hol_fmt         "hol%d"

#define hol_off         "%ld+hol%d"

#define con_cst(x)      fprintf(codefile,".data2\t%ld\n",x)
#define con_ilb(x)      fprintf(codefile,".data2\t%s\n",x)
#define con_dlb(x)      fprintf(codefile,".data2\t%s\n",x)

#define modhead         ".sect .zero; .sect .text; .sect .rom; .sect .data; .sect .bss\n"

#define fmt_id(ft, fs)	sprintf(fs,"_%s",ft)

#define BSS_INIT        0

void con_float(void);
void con_mult(word sz);
void con_part(register int sz, word w);
void mes(word type);
void prolog(full nlocals);


