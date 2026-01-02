/*
 * tk like ui
 */
void draw_scene( void );
void idle( void );
int key(int k, int mask);
void reshape( int width, int height );
void init( void );
int ui_loop(int argc, char **argv, const char *name);
void tkSwapBuffers(void);

#define KEY_UP     0xe000
#define KEY_DOWN   0xe001
#define KEY_LEFT   0xe002
#define KEY_RIGHT  0xe003
#define KEY_ESCAPE 0xe004

