/* Copyright (c) 1998 Lucent Technologies - All rights reserved. */
#include <u.h>
#include <libc.h>
#include <libg.h>
#include <frame.h>

void
frselect(Frame *f, Mouse *m)	/* when called, button 1 is down */
{
	ulong p0, p1, q;
	Point mp, pt0, pt1, qt;

	mp = m->xy;

    Again:
	f->modified = 0;
	frselectp(f, F&~D);
	p0 = p1 = frcharofpt(f, mp);
	pt0 = frptofchar(f, p0);
	pt1 = frptofchar(f, p1);
	frselectf(f, pt0, pt1, F&~D);
	do{
		if(f->modified)	/* special hack so 8½ can frselect in parallel */
			goto Again;
		q = frcharofpt(f, m->xy);
		if(p1 != q){
			if(p0 == p1)
				frselectf(f, pt0, pt1, F&~D);
			qt = frptofchar(f, q);
			if(p1 < q)
				frselectf(f, pt1, qt, F&~D);
			else
				frselectf(f, qt, pt1, F&~D);
			p1 = q;
			pt1 = qt;
			if(p0 == p1)
				frselectf(f, pt0, pt1, F&~D);
		}
		f->modified = 0;
		if(p0 < p1)
			f->p0 = p0, f->p1 = p1;
		else
			f->p0 = p1, f->p1 = p0;
		frgetmouse();
	}while((m->buttons & 7) == 1);
}
/* it is assumed p0<=p1 and both were generated by frptofchar() */
void
frselectf(Frame *f, Point p0, Point p1, Fcode c)
{
	int n;
	Point q0, q1;

	if(p0.x == f->left)
		p0.x = f->r.min.x;
	if(p1.x == f->left)
		p1.x = f->r.min.x;
	q0 = p0;
	q1 = p1;
	q0.y += f->font->height;
	q1.y += f->font->height;
	n = (p1.y-p0.y)/f->font->height;
	if(f->b == 0)
		berror("frselectf b==0");
	if(p0.y == f->r.max.y)
		return;
	if(n == 0){
		if(p0.x == p1.x)
			if(p0.x == f->r.min.x)
				q1.x++;
			else
				p0.x--;
		bitblt(f->b, p0, f->b, Rpt(p0, q1), c);
	}else{
		if(p0.x >= f->r.max.x)
			p0.x = f->r.max.x-1;
		bitblt(f->b, p0, f->b, Rect(p0.x, p0.y, f->r.max.x, q0.y), c);
		if(n > 1)
			bitblt(f->b, Pt(f->r.min.x, q0.y),
				f->b, Rect(f->r.min.x, q0.y, f->r.max.x, p1.y), c);
		bitblt(f->b, Pt(f->r.min.x, p1.y),
				f->b, Rect(f->r.min.x, p1.y, q1.x, q1.y), c);
	}
}

void
frselectp(Frame *f, Fcode c)
{
	Point pt0, pt1;

	pt0 = frptofchar(f, f->p0);
	pt1 = (f->p0==f->p1)? pt0 : frptofchar(f, f->p1);
	frselectf(f, pt0, pt1, c);
}