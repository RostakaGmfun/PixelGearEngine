/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGH
#define PXGH

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <glm/fwd.hpp>
#include <glm/vec4.hpp>

#include <string>
#include <vector>
#include <fstream>

class pxgHUDManager;
class pxgApp;
class pxgTexturePool;
union SDL_Event;
struct SDL_Window;
typedef uint8_t Uint8;
typedef uint32_t Uint32;
typedef int32_t SDL_Keycode;


class pxgEngine
{
public:
    void RunApp(pxgApp* app);
    void RunAppSDL(pxgApp* app);
	void Run(int width, int height, const char *title, bool fs);
	void RunXCB(int width, int height, const char* title, bool fs);
	void LoopFunc(void(*f)(void)) { loop_func = f; }
	void InitFunc(void(*f)(void)) { init_func = f; }
	void ResizeFunc(void(*f)(int,int)) { resize_func = f; }
	void SetFullScreen(bool fullscreen);
    void SDLSetFullscreen(bool fullscreen);
    bool SDLIsFullscreen();
    int SDLGetDesktopWidth();
    int SDLGetDesktopHeight();
	void SetBGColor(glm::vec4 color) { glClearColor(color.x,color.y,color.z,color.w); }
	int MouseX();
	int MouseY();
    bool MouseLB();
    bool MouseLBClick();
    bool MouseRB();
    bool MouseRBClick();
    bool MouseMB();
    bool MouseMBClick();
    int MouseWheel();

    static pxgTexturePool* TexturePool();

	void SetHUDManager(pxgHUDManager* man) { hudManager = man; }

	bool KeyDown(unsigned char key);
    bool SDLKeyDown(int key);
    bool SDLKeyPressed(SDL_Keycode key);
	bool KeyPressed(unsigned char key);

	void ProcessKeys(XKeyEvent event);

    int GetTime();
    float GetFPS() const { return fps; }


    void InputLoop();

    void SetCursorPos(int x, int y);

static int speed;

private:

Display *dpy;
Window root;
XVisualInfo *vi;
Colormap cmap;
XSetWindowAttributes swa;
Window win;
GLXContext glc;
XWindowAttributes gwa;
XEvent xev;

bool keys[256];
bool prev_keys[256];
bool pressed_keys[256];
const Uint8* sdlKeys;
std::vector<SDL_Keycode> sdlPressedKeys;
std::vector<SDL_Keycode> sdlWasPress;
bool sdl_lmb_press;
bool sdl_lmb_wasPress;
bool sdl_rmb_press;
bool sdl_rmb_wasPress;
bool sdl_mmb_press;
bool sdl_mmb_wasPress;
Uint32 mouseState;
int Mx, My;
int wheel;
SDL_Window* window;

float fps;
//int frames;
int start_time;

int width, height;
bool fullscreen;

pxgHUDManager* hudManager;

void(*loop_func)(void);
void(*init_func)(void);
void(*resize_func)(int, int);

void DrawHUD();
void ProcessSDLKeys(SDL_Event* event);

enum FRAMEWORK_API
{
    API_X11,
    API_SDL2
};

FRAMEWORK_API currentAPI;


};

#endif //PXGH
