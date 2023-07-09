#include <u.h>
#include <libc.h>
#include "thread.h"

#define nprocs		4
#define nthreads 4

#define STACKSIZE (512*1024)

extern ulong rendezvouses;

Channel *gc[nthreads];
char *progname;

#define BSIZE 20

typedef struct {
	uint len;
	uint num;
	char data[BSIZE];
} Buf;

void
readthread(ulong ch) {
	Channel *c;
	Buf buf;
	int n;
	int i;

	c = (Channel *)ch;
	i = 0;
	do {
		n = read(0, buf.data, BSIZE);
		if (n < 0)
			buf.len = 0;
		else
			buf.len = n;
		buf.num = i++;
		send(c, &buf);
	} while (n > 0);
	if (n < 0) {
		fprint(2, "read: %r\n");
		exits("read");
	}
	exits(0);
}

void
writethread(ulong ch) {
	Channel *c;
	Buf buf;
	int i;

	c = (Channel *)ch;
	i = 0;
	while(1) {
		recv(c, &buf);
		if (buf.len == 0) break;
		if (buf.len != BSIZE) fprint(2, "Unusual bufsize %d\n", buf.len);
		if (buf.num != i)
			fprint(2, "Out of order: got %d, exp %d\n", buf.num, i);
		i = buf.num + 1;
		if (write(1, buf.data, buf.len) != buf.len) {
			fprint(2, "write: %r\n");
			exits("write");
		}
	}
	exits(0);
}

void
testproc(ulong id) {
	int i;
	Proc *p;
	Channel *c;

	p = *procp;
	p->pid = id;

	progname = argv[0];

	if ((c = newchan(sizeof(Buf), 100)) == nil)
		fatal("Newchan returned nil");
	newproc(readthread, (ulong)c, RFPROC|RFMEM, STACKSIZE);
	sleep(1);
	newproc(writethread, (ulong)c, RFPROC|RFMEM, STACKSIZE);
	while (wait(&w) > 0)
		;
	fprint(2, "Rendezvouses = %lud\n", rendezvouses);
}

void
main(int argc, char *argv[]) {
	int i, j;
	Proc *p;
	Waitmsg w;
	progname = argv[0];

	procinit(&p);	/* Initialize proc data structures */

	/* Three channels, make channel 0 buffered */
	for (i = 0; i < nprocs; i++) {
		if (newproc(testproc, i, STACKSIZE) == nil) {
			print("Can't start proc: %r\n");
			exits("procerror");
		}
	}
	while (wait(&w) > 0)
		;
}
