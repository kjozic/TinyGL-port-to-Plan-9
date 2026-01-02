/* simple glx like driver for TinyGL and Plan 9 */

#include <u.h>
#include <libc.h>
#include <draw.h>
#include <GL/plan9.h>
#include "zgl.h"

Plan9Context *plan9CreateContext(char *label)
{
  Plan9Context *ctx = gl_malloc(sizeof(Plan9Context));

  if (!ctx)
    return NULL;

  if (initdraw(nil, "*default*", label) < 0)
  {
    free(ctx);

    return NULL;
  }

  return ctx;
}

void plan9DestroyContext()
{
  GLContext *gl_context;
  Plan9Context *ctx;

  gl_context = gl_get_context();
  ctx = (Plan9Context *)gl_context->opaque;

  if (ctx != NULL)
  {
    free(ctx);
  }

  glClose();
  closedisplay(display);
}

static int plan9_resize_viewport(GLContext *c, int *xsize_ptr, int *ysize_ptr)
{
  Plan9Context *ctx;
  int xsize, ysize;

  ctx = (Plan9Context *)c->opaque;

  xsize = *xsize_ptr;
  ysize = *ysize_ptr;

  /* We ensure that xsize and ysize are multiples of 4 for the zbuffer. */
  xsize &= ~3;
  ysize &= ~3;

  if (xsize == 0 || ysize == 0)
    return -1;

  *xsize_ptr = xsize;
  *ysize_ptr = ysize;

  ctx->xsize = xsize;
  ctx->ysize = ysize;

  /* resize the Z buffer */
  ZB_resize(c->zb, NULL, xsize, ysize);
  return 0;
}

int plan9MakeCurrent(Plan9Context *ctx)
{
  GLContext *gl_context;
  ZBuffer *zb;
  int xsize, ysize;

  if (ctx != NULL)
  {
    xsize = screen->r.max.x - screen->r.min.x;
    ysize = screen->r.max.y - screen->r.min.y;

    zb = ZB_open(xsize, ysize, ZB_MODE_5R6G5B, 0, NULL, NULL, NULL);

    if (zb == NULL)
    {
      fprintf(stderr, "Error while initializing Z buffer\n");
      exit(1);
    }

    glInit(zb);

    gl_context = gl_get_context();
    gl_context->opaque = ctx;
    gl_context->gl_resize_viewport = plan9_resize_viewport;

    /* set the viewport : we force a call to glX_resize_viewport */
    gl_context->viewport.xsize = -1;
    gl_context->viewport.ysize = -1;

    glViewport(0, 0, xsize, ysize);
  }

  return 1;
}

void plan9SwapBuffers()
{
  GLContext *gl_context;
  Plan9Context *ctx;
  Image *img;

  gl_context = gl_get_context();
  ctx = (Plan9Context *)gl_context->opaque;
  img = allocimage(display, Rect(0, 0, ctx->xsize, ctx->ysize), RGB16, 0, DNofill);

  if (!img)
  {
    printf("Could not allocate image");

    return;
  }

  loadimage(img, Rect(0, 0, ctx->xsize, ctx->ysize), (void *)gl_context->zb->pbuf, ctx->xsize * ctx->ysize * 2);
  draw(screen, Rect(0, 0, ctx->xsize, ctx->ysize), img, nil, ZP);
  flushimage(display, 1);
  freeimage(img);
}
