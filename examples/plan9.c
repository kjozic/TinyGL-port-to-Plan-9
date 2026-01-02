/*
 * Demonstration program for Plan 9 graphics.
 */

#include <stdio.h>
#include <u.h>
#include <fmt.h>
#include <libc.h>
#include <draw.h>
#include <event.h>
#include <GL/plan9.h>
#include "ui.h"

void tkSwapBuffers(void)
{
    plan9SwapBuffers();
}

int ui_loop(int argc, char **argv, const char *name)
{
    Plan9Context *ctx;
    Event e;
    ulong timer, evtype;
    int pressedkey;

    ctx = (Plan9Context *)plan9CreateContext(argv[0]);

    if (!ctx)
    {
        printf("Could not create context");

        return 0;
    }

    if (!plan9MakeCurrent(ctx))
    {
        printf("Could not make context current");

        return 0;
    }

    einit(Ekeyboard);
    eresized(0);

    timer = etimer(0, 35);

    init();
    reshape(ctx->xsize, ctx->ysize);

    while (1)
    {
        evtype = event(&e);

        if (evtype == Ekeyboard)
        {
            pressedkey = e.kbdc;

            /* printf("Keycode: %u\n", pressedkey); */

            switch (pressedkey)
            {
            case 61457:
                key(KEY_LEFT, 0);
                break;
            case 61458:
                key(KEY_RIGHT, 0);
                break;
            case 61454:
                key(KEY_UP, 0);
                break;
            case 128:
                key(KEY_DOWN, 0);
                break;
            case 27:
                exit(0);
                break;
            default:
                key(pressedkey, 0);
            }
        }
        else if (evtype == timer)
        {
            idle();
        }
    }

    return 0;
}

void eresized(int new)
{
    getwindow(display, Refnone);
}
