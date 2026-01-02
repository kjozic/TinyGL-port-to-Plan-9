#ifndef PLAN9_H
#define PLAN9_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  int xsize;
  int ysize;
} Plan9Context;

extern Plan9Context *plan9CreateContext(char *label);

extern void plan9DestroyContext();

extern int plan9MakeCurrent(Plan9Context *ctx);

extern void plan9SwapBuffers();

#ifdef __cplusplus
}
#endif

#endif
