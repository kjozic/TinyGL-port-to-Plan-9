/*
 * Demonstration program for Nano-X graphics.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <draw.h>
#include <event.h>
#include <GL/gl.h>
#include <GL/plan9.h>
#include "ui.h"

void tkSwapBuffers(void)
{
    plan9SwapBuffers(w1);
}

int ui_loop(int argc, char **argv, const char *name)
{
    Plan9Context *ctx;
    Event e;
    ulong evtype;

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

    einit(Emouse | Ekeyboard);
    eresized(0);

    init();
    reshape(ctx->xsize, ctx->ysize);

    while (1)
    {
        evtype = event(&e);

        if (evtype == Emouse)
        {
            plan9DestroyContext();
        }

        else if (evtype == Ekeyboard)
        {
            key = e.kbdc;

            /* left arrow key */
            if (key == 61457)
            {
                key(KEY_LEFT, 0);
            }

            /* right arrow key */
            if (key == 61458)
            {
                key(KEY_RIGHT, 0);
            }

            /* up arrow key */
            if (key == 61459)
            {
                key(KEY_UP, 0);
            }

            /* down arrow key */
            if (key == 61460)
            {
                key(KEY_DOWN, 0);
            }


            /* esc arrow key */
            if (key == 61460)
            {
                plan9DestroyContext();
            }
        }

        idle();
    }

    return 0;
}

void eresized(int new)
{
    /* just refresh the window for now. */
    getwindow(display, Refnone);
}

Mouse emouse(void)
{
    plan9DestroyContext();
}

int ekbd(void)
{
    plan9DestroyContext();
}