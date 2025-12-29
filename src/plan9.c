/* simple glx like driver for TinyGL and Nano X */
#include <GL/gl.h>
#include <GL/plan9.h>
#include "zgl.h"

void *plan9CreateContext(char *label)
{
  Plan9Context *ctx;

  ctx = gl_malloc(sizeof(Plan9Context));

  if (!ctx)
    return NULL;

  ctx->gl_context = NULL;

  if (initdraw(nil, nil, label) < 0)
  {
    free(ctx);

    return NULL;
  }

  if (screen->depth != 16)
  {
    free(ctx);

    return NULL;
  }

  return (void *)ctx;
}

void plan9DestroyContext(Plan9Context *ctx)
{
  if (ctx->gl_context != NULL)
  {
    glClose();
  }

  free(ctx);

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
  int mode, xsize, ysize;
  ZBuffer *zb;

  if (ctx->gl_context == NULL)
  {
    xsize = screen->max.x - screen->min.x;
    ysize = screen->max.y - screen->min.y;

    /* currently, we only support 16 bit rendering */
    mode = ZB_MODE_5R6G5B;
    zb = ZB_open(xsize, ysize, mode, 0, NULL, NULL, NULL);

    if (zb == NULL)
    {
      fprintf(stderr, "Error while initializing Z buffer\n");
      exit(1);
    }

    ctx->pixtype = MWPF_TRUECOLOR565;

    glInit(zb);

    ctx->gl_context = gl_get_context();
    ctx->gl_context->opaque = (void *)ctx;
    ctx->gl_context->gl_resize_viewport = plan9_resize_viewport;

    /* set the viewport : we force a call to glX_resize_viewport */
    ctx->gl_context->viewport.xsize = -1;
    ctx->gl_context->viewport.ysize = -1;

    glViewport(0, 0, xsize, ysize);
  }

  return 1;
}

void plan9SwapBuffers()
{
  GLContext *gl_context;

  gl_context = gl_get_context();
  ctx = (Plan9Context *)gl_context->opaque;

  Image *img;
  Rectangle rec;

  rec = Rect(0, 0, ctx->xsize, ctx->ysize);

  /*
  rec.min.x = 0;
  rec.min.y = 0;
  rec.max.x = ctx->xsize;
  rec.max.y = ctx->ysize;
  */

  img = allocimage(display, rec, RGB16, 0, DNofill);

  if (!img)
  {
    printf("Could not allocate image");

    return;
  }

  loadimage(img, rec, (uchar *)ctx->gl_context->zb->pbuf, ctx->xsize * ctx->ysize * 2);
  draw(screen, rec, img, nil, ZP);
  freeimage(img);
}
