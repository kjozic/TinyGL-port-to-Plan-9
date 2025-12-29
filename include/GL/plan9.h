#ifndef PLAN9_H
#define PLAN9_H

#include <draw.h>
#include <GL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  GLContext *gl_context;
  int xsize;
  int ysize;
  int pixtype;
} Plan9Context;

extern void *plan9CreateContext(char *label);

extern void plan9DestroyContext(Plan9Context *ctx);

extern int plan9MakeCurrent(Plan9Context *ctx);

extern void plan9SwapBuffers();

#ifdef __cplusplus
}
#endif

#endif
