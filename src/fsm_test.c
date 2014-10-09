#include "fsm.h"
#include <stdio.h>

void
err_hander(int state, int event)
{
	printf("can't trans from %d via %d\n", state, event);
}

void
trans_hander(int from, int event, int to)
{
	printf("from %d via %d to %d\n", from, event, to);
}

int
main(int argc, char *argv[])
{
	fsm_t fsm = makeFSMType(err_hander);
	fsmSetHander(&fsm, 2, trans_hander);
	fsmSetHander(&fsm, 1, trans_hander);
	fsmSetHander(&fsm, 0, trans_hander);
	fsmSetCurState(&fsm, 0);
	fsmLink(&fsm, 0, 0, 0);
	fsmLink(&fsm, 0, 1, 1);
	fsmLink(&fsm, 0, 2, 2);
	fsmLink(&fsm, 1, 0, 0);
	fsmLink(&fsm, 1, 1, 1);
	fsmLink(&fsm, 1, 2, 2);
	fsmLink(&fsm, 2, 0, 0);
	fsmLink(&fsm, 2, 1, 1);
	fsmLink(&fsm, 2, 2, 2);
	char ch;
	while((ch = getchar()) != EOF) {
		if(ch == '\n')
			continue;
		printf("push %d\n", ch - '0');
		fsmPushEvent(&fsm, ch - '0');
	}
	destroyFSMType(fsm);
	return 0;
}
