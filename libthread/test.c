#include <u.h>
#include <libc.h>
#include <fatal.h>
#include <sched.h>

#define Stacksize (64*1024)

int
child(void *arg) {
	long l;

	fprint(2, "Child\n");
	l = 0xbedbed;
	l = rendezvous(0xdeadead, l);
	fprint(2, "Child rendezvous 0x%lx\n", l);
	exit(0);
}

void
main(int argc, char *argv[]) {
	char *stack;
	long l;

	fprint(2, "Main\n");

	stack = (char *)malloc(Stacksize);
	memset(stack, 0, Stacksize);

	switch(__clone(child, stack+Stacksize, CLONE_VM, nil)) {
	case -1:
		fatal("Can't fork");
	case 0:
		fatal("Can't happen");
	default:
		fprint(2, "Parent\n");
		break;
	}
	l = 0xbadbad;
	l = rendezvous(0xdeadead, l);
	fprint(2, "Parent rendezvous 0x%lx\n", l);
	exit(0);
}
