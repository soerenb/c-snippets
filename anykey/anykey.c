/*
 * How to wait for ANY key. Apparently not a trivial task for c. This example
 * implements a solution.
 * Code was copied from: http://musicdieu.livejournal.com/135980.html
 */
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char b;
	struct termios tcattr;

	/* the spacing of the ellipsis matters if you want to look like DOS */
	printf("Press any key to continue . . .\n");
	tcgetattr(0, &tcattr);

	/* disable 'Canonical mode' which assumes you want line-buffering */
	tcattr.c_lflag &= ~ICANON;

	/*
	 * Set TIME = 0, and MIN = 1, thus telling the terminal that we want to
	 * send along the stdin buffer when a minimum of MIN or chars requested
	 * by read() is available, ie. in our case 1 (since read() just returns
	 * immediately if you want 0 chars), and to disable the timeout which
	 * governs how long to wait before sending the stdin buffer along.
	 */
	tcattr.c_cc[VTIME] = 0;
	tcattr.c_cc[VMIN] = 1;

	/* engage - NOW! */
	tcsetattr(0, TCSANOW, &tcattr);

	read(0, &b, 1);

	return 0;
}
