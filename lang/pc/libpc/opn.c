/* $Header$ */
/*
 * (c) copyright 1983 by the Vrije Universiteit, Amsterdam, The Netherlands.
 *
 *          This product is part of the Amsterdam Compiler Kit.
 *
 * Permission to use, sell, duplicate or disclose this software must be
 * obtained in writing. Requests for such permissions may be sent to
 *
 *      Dr. Andrew S. Tanenbaum
 *      Wiskundig Seminarium
 *      Vrije Universiteit
 *      Postbox 7161
 *      1007 MC Amsterdam
 *      The Netherlands
 *
 */

/* Author: J.W. Stevenson */

#include	<pc_file.h>
#include	<pc_err.h>

extern struct file	**_extfl;
extern int		_extflc;
extern struct file	*_curfil;
extern int		_pargc;
extern char		**_pargv;
extern char		**_penvp;

extern			_cls();
extern			_xcls();
extern			_trp();
extern int		getpid();
extern int		creat();
extern int		open();
extern int		close();
extern int		unlink();
extern long		lseek();

static int tmpfil() {
	int i; char *p,*q;

	i = getpid();
	p = "/usr/tmp/plf.xxxxx";
	q = p + 13;
	do
		*q++ = (i & 07) + '0';
	while (i >>= 3);
	*q = '\0';
	if ((i = creat(p,0644)) < 0)
		if ((i = creat(p += 4,0644)) < 0)
			if ((i = creat(p += 5,0644)) < 0)
				goto error;
	if (close(i) != 0)
		goto error;
	if ((i = open(p,2)) < 0)
		goto error;
	if (unlink(p) != 0)
error:		_trp(EREWR);
	return(i);
}

static int initfl(descr,sz,f) int descr; int sz; struct file *f; {
	int i;

	_curfil = f;
	if (sz == 0) {
		sz++;
		descr |= TXTBIT;
	}
	for (i=0; i<_extflc; i++)
		if (f == _extfl[i])
			break;
	if (i >= _extflc) {		/* local file */
		f->fname = "LOCAL";
		if ((descr & WRBIT) == 0 && (f->flags & 0377) == MAGIC) {
			_xcls(f);
			if (lseek(f->ufd,(long)0,0) == -1)
				_trp(ERESET);
		} else {
			_cls(f);
			f->ufd = tmpfil();
		}
	} else {	/* external file */
		if (--i <= 0)
			return(0);
		if (i >= _pargc)
			_trp(EARGC);
		f->fname = _pargv[i];
		_cls(f);
		if ((descr & WRBIT) == 0) {
			if ((f->ufd = open(f->fname,0)) < 0)
				_trp(ERESET);
		} else {
			if ((f->ufd = creat(f->fname,0644)) < 0)
				_trp(EREWR);
		}
	}
	f->buflen = (sz>PC_BUFLEN ? sz : PC_BUFLEN-PC_BUFLEN%sz);
	f->size = sz;
	f->ptr = f->bufadr;
	f->flags = descr;
	return(1);
}

_opn(sz,f) int sz; struct file *f; {

	if (initfl(MAGIC,sz,f))
		f->count = 0;
}

_cre(sz,f) int sz; struct file *f; {

	if (initfl(WRBIT|EOFBIT|ELNBIT|MAGIC,sz,f))
		f->count = f->buflen;
}
