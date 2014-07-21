#include <iostream>
#include <PXG.h>
#include <cstdlib>
/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <chrono>
#include <thread>
#include <X11/cursorfont.h>
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>

#include <stdio.h>
#include <algorithm>

#include <glm/glm.hpp>

#include <sys/time.h>

#include <pxgHUDManager.h>
#include <pxgApp.h>
#include <pxgTexturePool.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL/SDL.h>
#include <pxgTools.h>

int pxgEngine::speed = -1;

void pxgEngine::InputLoop()
{
    while(1)
    {
       //XNextEvent(dpy,&xev);
        switch(xev.type)
        {
            case KeyPress:
            {
                keys[xev.xkey.keycode] = true;
                break;
            }
            case KeyRelease:
            {
                keys[xev.xkey.keycode] = false;
                break;
            }
            case MotionNotify:
            {
                Mx = xev.xmotion.x;
                My = xev.xmotion.y;
                break;
            }
        }
       //std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    }
}

void pxgEngine::ProcessKeys(XKeyEvent event)
{
    if(currentAPI!=API_X11)
        return;
    char r_keys[256];
    int numRecieved;
    KeySym ks;
    numRecieved = XLookupString(&event,r_keys,256,&ks,NULL);
    if(numRecieved<=0)
        return;


    if(event.type == KeyPress)
        for(int i = 0;i<numRecieved;i++)
        {
            keys[r_keys[i]] = true;
           // if(!prev_keys[r_keys[i]])
           //     pressed_keys[r_keys[i]] = true;
        }

    if(event.type == KeyRelease)
        for(int i = 0;i<numRecieved;i++)
        {
            keys[r_keys[i]] = false;
        }

    for(int i = 0;i<256;i++)
        prev_keys[i] = keys[i];

}


void pxgEngine::Run(int Width, int Height, const char *title, bool fs)
{
    currentAPI = API_X11;
    PXG::InitLog();
    PXG::Log("Initialising PXG engine...", OBLIGATORY);
	pxgInitExtensions();
	width = Width;
	height = Height;
    dpy = XOpenDisplay(NULL);
   // PFNGLXSWAPINTERVALMESAPROC proc = (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddress((GLubyte*)"glxSwapIntervalMESA");
    if(!dpy)
        return;
    root = DefaultRootWindow(dpy);
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    vi = glXChooseVisual(dpy, 0, att);
    if(vi==NULL)
        return;
    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ButtonPressMask|StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask|PointerMotionMask;
    //swa.override_redirect = true;
    win = XCreateWindow(dpy, root, 0, 0, Width, Height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XSelectInput(dpy, win, ButtonPressMask|StructureNotifyMask|KeyPressMask|KeyReleaseMask|PointerMotionMask);
    //XGrabKeyboard(dpy, win, true, GrabModeAsync, GrabModeAsync, CurrentTime);
    //XGrabPointer(dpy,win,true,PointerMotionMask,GrabModeSync,GrabModeAsync,None,None,CurrentTime);
    XMapWindow(dpy, win);
    XMoveWindow(dpy,win,1920/2-width/2,1080/2-height/2);
	XStoreName(dpy, win, title);
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);

    PXG::Log("Created glx context with double buffering", OBLIGATORY);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	if(init_func!=NULL)
		init_func();
    SetFullScreen(fs);

    int frames, sec0, usec0, sec, dusec, usec;
    timeval now;
    clock_t t1, t2;
    clock_t overtime, nextUpdate;

    unsigned long mStart, mEnd;

    int frame = 0;

    for(int i = 0;i<256;i++)
    {
        keys[i] = false;
        prev_keys[i] = false;
    }

    //std::chrono::seconds start, end;
    time_t start, end;
	while(1)
	{
            mStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            XCheckWindowEvent(dpy,win,KeyPressMask|KeyReleaseMask|PointerMotionMask|StructureNotifyMask|KeymapNotify,&xev);
                switch(xev.type)
                {
                    case KeymapNotify:
                        XRefreshKeyboardMapping(&xev.xmapping);
                    break;
                    case ConfigureNotify:
                    {
                        if (xev.xconfigure.width != width || xev.xconfigure.height != height)
                        {
                            width = xev.xconfigure.width;
                            height = xev.xconfigure.height;
                            glViewport(0,0,width,height);
                            if(resize_func!=NULL) resize_func(width, height);
                        }
                        break;
                    }
                    case MotionNotify:
                    {
                        Mx = xev.xmotion.x;
                        My = xev.xmotion.y;
                        XCheckWindowEvent(dpy,win,KeyPressMask|KeyReleaseMask,&xev);
                    }
                    case KeyPress:
                    {
                        ProcessKeys(xev.xkey);
                        break;
                    }
                    case KeyRelease:
                    {
                        ProcessKeys(xev.xkey);
                        break;
                    }

                }
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                if(loop_func!=NULL)
					loop_func();
                if(hudManager!=NULL)
                    DrawHUD();
                glXSwapBuffers(dpy, win);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000/speed));
                mEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                frame++;
                if(frame==10)
                {
                    frame = 0;
                    fps = 1.f/(float(mEnd-mStart)/1000);
                }
                for(int i = 0;i<256;i++)
                    pressed_keys[i] = false;

	}

 glXMakeCurrent(dpy, None, NULL);
 glXDestroyContext(dpy, glc);
 XDestroyWindow(dpy, win);
 XCloseDisplay(dpy);
}

void pxgEngine::RunApp(pxgApp* app)
{
    if(!app)
        return;
    currentAPI = API_X11;
    PXG::InitLog();
    PXG::Log("Initialising PXG engine...", OBLIGATORY);
    pxgInitExtensions();
    width = app->Width();
    height = app->Height();
    dpy = XOpenDisplay(NULL);
   // PFNGLXSWAPINTERVALMESAPROC proc = (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddress((GLubyte*)"glxSwapIntervalMESA");
    if(!dpy)
        return;
    root = DefaultRootWindow(dpy);
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    vi = glXChooseVisual(dpy, 0, att);
    if(vi==NULL)
        return;
    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ButtonPressMask|StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask|PointerMotionMask;
    //swa.override_redirect = true;
    win = XCreateWindow(dpy, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XSelectInput(dpy, win, ButtonPressMask|StructureNotifyMask|KeyPressMask|KeyReleaseMask|PointerMotionMask);
    //XGrabKeyboard(dpy, win, true, GrabModeAsync, GrabModeAsync, CurrentTime);
    //XGrabPointer(dpy,win,true,PointerMotionMask,GrabModeSync,GrabModeAsync,None,None,CurrentTime);
    XMapWindow(dpy, win);
    XMoveWindow(dpy,win,1920/2-width/2,1080/2-height/2);
    XStoreName(dpy, win, app->Title().c_str());
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);

    PXG::Log("Created glx context with double buffering", OBLIGATORY);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    SetFullScreen(app->Fullscreen());
    app->Init();


    unsigned long mStart, mEnd;

    int frame = 0;

    for(int i = 0;i<256;i++)
    {
        keys[i] = false;
        prev_keys[i] = false;
    }

    //std::chrono::seconds start, end;
    while(1)
    {
            mStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            XCheckWindowEvent(dpy,win,KeyPressMask|KeyReleaseMask|PointerMotionMask|StructureNotifyMask|KeymapNotify,&xev);
                switch(xev.type)
                {
                    case KeymapNotify:
                        XRefreshKeyboardMapping(&xev.xmapping);
                    break;
                    case ConfigureNotify:
                    {
                        if (xev.xconfigure.width != width || xev.xconfigure.height != height)
                        {
                            width = xev.xconfigure.width;
                            height = xev.xconfigure.height;
                            glViewport(0,0,width,height);
                            if(resize_func!=NULL) resize_func(width, height);
                        }
                        break;
                    }
                    case MotionNotify:
                    {
                        Mx = xev.xmotion.x;
                        My = xev.xmotion.y;
                        XCheckWindowEvent(dpy,win,KeyPressMask|KeyReleaseMask,&xev);
                    }
                    case KeyPress:
                    {
                        ProcessKeys(xev.xkey);
                        break;
                    }
                    case KeyRelease:
                    {
                        ProcessKeys(xev.xkey);
                        break;
                    }

                }
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                if(!app->Frame())
                    break;
                if(hudManager!=NULL)
                    DrawHUD();
                glXSwapBuffers(dpy, win);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000/speed));
                mEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                frame++;
                if(frame==10)
                {
                    frame = 0;
                    fps = 1.f/(float(mEnd-mStart)/1000);
                }
                for(int i = 0;i<256;i++)
                    pressed_keys[i] = false;

    }
 app->Destroy();
 glXMakeCurrent(dpy, None, NULL);
 glXDestroyContext(dpy, glc);
 XDestroyWindow(dpy, win);
 XCloseDisplay(dpy);
}

void pxgEngine::RunAppSDL(pxgApp *app)
{
    if(!app)
        return;
    currentAPI = API_SDL2;
    PXG::InitLog();
    PXG::Log("Initialising PXG engine...", OBLIGATORY);
    pxgInitExtensions();

    SDL_GLContext context;

    width = app->Width();
    height = app->Height();

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        PXG::Log("PXG::RunAppSDL(): error initialising SDL",ERR);
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, app->GLMajor());
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, app->GLMinor());

    if(app->DBuffer())
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    window = SDL_CreateWindow(app->Title().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if(!window)
    {
        PXG::Log("pxgRunAppSDL(): error creatinf window", ERR);
        return;
    }


    context = SDL_GL_CreateContext(window);

    if(app->VSync())
        SDL_GL_SetSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    PXG::Log("PXG::RunAppSDL(): sucessfully initalised");

    app->Init();

    unsigned long mStart, mEnd;

    int frame = 0;

    sdlKeys = SDL_GetKeyboardState(NULL);
    sdl_lmb_wasPress = false;
    sdl_rmb_wasPress = false;
    sdl_mmb_wasPress = false;
    sdlWasPress.clear();
   // SDL_SemTryWait
    bool quit = false;
    while(1)
    {
        mStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        SDL_Event event;
        sdl_lmb_press = false;
        sdl_rmb_press = false;
        sdl_mmb_press = false;
        sdlPressedKeys.clear();
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                {
                    if(std::find(sdlWasPress.begin(),sdlWasPress.end(),event.key.keysym.sym)==sdlWasPress.end())
                    {
                        sdlPressedKeys.push_back(event.key.keysym.sym);
                        sdlWasPress.push_back(event.key.keysym.sym);
                    }
                    break;
                }
                case SDL_KEYUP:
                {
                    if(std::find(sdlWasPress.begin(),sdlWasPress.end(),event.key.keysym.sym)!=sdlWasPress.end())
                        sdlWasPress.erase(std::find(sdlWasPress.begin(),sdlWasPress.end(),event.key.keysym.sym));
                    break;
                }
                case SDL_MOUSEWHEEL:
                {
                    wheel = event.wheel.y;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                switch(event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    if(!sdl_lmb_wasPress)
                    {
                        sdl_lmb_wasPress = true;
                        sdl_lmb_press = true;
                    }
                    break;
                case SDL_BUTTON_RIGHT:
                    if(!sdl_rmb_wasPress)
                    {
                        sdl_rmb_wasPress = true;
                        sdl_rmb_press = true;
                    }
                    break;
                case SDL_BUTTON_MIDDLE:
                    if(!sdl_mmb_wasPress)
                    {
                        sdl_mmb_wasPress = true;
                        sdl_mmb_press = true;
                    }
                    break;
                }
                break;
                }
            case SDL_MOUSEBUTTONUP:
            {
                switch(event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    sdl_lmb_wasPress = false;
                    break;
                case SDL_BUTTON_RIGHT:
                    sdl_rmb_wasPress = false;
                    break;
                case SDL_BUTTON_MIDDLE:
                    sdl_mmb_wasPress = false;
                    break;
                }
                break;
            }
                case SDL_QUIT:
                {
                    quit = true;
                    break;
                }
            }
        }
        mouseState = SDL_GetMouseState(&Mx,&My);
        if(quit)
            break;
        if(!app->Frame())
            break;
        if(hudManager!=NULL)
            DrawHUD();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/speed));
        SDL_GL_SwapWindow(window);
        mEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        frame++;
        if(frame==80)
        {
            frame = 0;
            fps = 1.f/(int(mEnd-mStart)/1000.f);
        }
        wheel = 0;
    }
    app->Destroy();
    SDLSetFullscreen(false);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

bool pxgEngine::SDLKeyDown(int key)
{
    if(currentAPI!=API_SDL2)
        return false;
    return sdlKeys[SDL_GetScancodeFromKey(key)];
}

bool pxgEngine::SDLKeyPressed(SDL_Keycode key)
{
    if(currentAPI!=API_SDL2)
        return false;
    for(int i = 0;i<sdlPressedKeys.size();i++)
    {
        if(sdlPressedKeys[i]==key)
        {
            return true;
        }
    }
    return false;
}

bool pxgEngine::SDLIsFullscreen()
{
    if(currentAPI!=API_SDL2)
        return false;
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN);
}

void pxgEngine::SDLSetFullscreen(bool fullscreen)
{
    if(currentAPI!=API_SDL2)
        return;

    if(fullscreen)
        SDL_SetWindowFullscreen(window,SDL_GetWindowFlags(window) | SDL_WINDOW_FULLSCREEN_DESKTOP);
    else
        SDL_SetWindowFullscreen(window,SDL_GetWindowFlags(window) & ~SDL_WINDOW_FULLSCREEN_DESKTOP);
}

int pxgEngine::SDLGetDesktopWidth()
{
    if(currentAPI != API_SDL2)
        return 0;
    int x = 0;
    SDL_GetWindowSize(window, &x,NULL);
    return x;
}

int pxgEngine::SDLGetDesktopHeight()
{
    if(currentAPI != API_SDL2)
        return 0;
    int y = 0;
    SDL_GetWindowSize(window, NULL,&y);
    return y;
}

void pxgEngine::RunXCB(int Width, int Height, const char* title, bool fs)
{
    PXG::InitLog();
    PXG::Log("PXG::RunXCB(): not implemented", ERR);
        return;
    PXG::Log("Initialising PXG engine...", OBLIGATORY);
	pxgInitExtensions();
	/*width = Width;
	height = Height;
    dpy = XOpenDisplay(0);
    if(!dpy)
    {
        PXG::Log("PXG::RunXCB() error: cannot open X display",ERR);
        return;
    }
    int default_screen = DefaultScreen(dpy);

    xcb_connection_t *connection = XGetXCBConnection(dpy);
    if(!connection)
    {
        XCloseDisplay(dpy);
        PXG::Log("PXG::RunXCB() error: cannont get XCB connection",ERR);
        return;
    }

    XSetEventQueueOwner(dpy, XCBOwnsEventQueue);

    xcb_screen_t *screen = 0;
    xcb_screen_iterator_t screenI = xcb_setup_roots_iterator(xcb_get_setup(connection));
        for(int i = default_screen; screenI.rem && i > 0;--i, xcb_screen_next(&screenI));
            screen = screenI.data;

    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

    int visualID = 0;
    int num_fb_configs = 0;
    GLXFBConfig* fb_configs = glXGetFBConfigs(dpy, default_screen, &num_fb_configs);
    if(!fb_configs || num_fb_configs == 0)
    {
        return;
    }

    GLXFBConfig fb_config = fb_configs[0];
    glXGetFBConfigAttrib(dpy, fb_config, GLX_VISUAL_ID , &visualID);

    vi = glXChooseVisual(dpy, 0, att);
    glc =  glXCreateNewContext(dpy, fb_config, GLX_RGBA_TYPE, 0, True);
    Log("Created glx context with double buffering", OBLIGATORY);

    xcb_colormap_t colormap = xcb_generate_id(connection);
    xcb_window_t window = xcb_generate_id(connection);

    xcb_create_colormap( connection, XCB_COLORMAP_ALLOC_NONE, colormap, screen->root,0);

    uint32_t eventmask = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
    uint32_t valuelist[] = { eventmask, colormap, 0 };
    uint32_t valuemask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;


        xcb_create_window(
            connection,
            XCB_COPY_FROM_PARENT,
            window,
            screen->root,
            0, 0,
            150, 150,
            0,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            0,
            valuemask,
            valuelist
            );


        // NOTE: window must be mapped before glXMakeContextCurrent
        xcb_map_window(connection, window);

        GLXDrawable drawable = 0;

        GLXWindow glxwindow =
            glXCreateWindow(
                dpy,
                fb_config,
                window,
                0
                );

         if(!window)
        {
            PXG::Log("PXG::RunXCB() error: cannotn create GLXWindow",ERR);
            xcb_destroy_window(connection, window);
            glXDestroyContext(dpy, glc);
            return;
        }
        drawable = glxwindow;
        //glXMakeCurrent(dpy,window,glc);
        /*if(!glXMakeContextCurrent(dpy, drawable, drawable, glc))
        {
             PXG::Log("PXG::RunXCB() error: cannot create GLX context current",ERR);
            xcb_destroy_window(connection, window);
            glXDestroyContext(dpy, glc);
            return;
        }*/
       /* while(1)
        {
            if(loop_func!=NULL)
                loop_func();
            //glXSwapBuffers(dpy,drawable);
        }*/
        //}*/

}

void pxgEngine::SetFullScreen(bool fs)
{
    if(currentAPI==API_X11)
    {
        fullscreen = fs;
        XWindowAttributes xwa;
        XGetWindowAttributes( dpy, win, &xwa );
		XEvent e;
        e.xclient.type = ClientMessage;
        e.xclient.message_type = XInternAtom( dpy, "_NET_WM_STATE", false );
        e.xclient.display = dpy;
        e.xclient.window = win;
        e.xclient.format = 32;
        e.xclient.data.l[0] = fullscreen;
        e.xclient.data.l[1] = XInternAtom( dpy, "_NET_WM_STATE_FULLSCREEN", false );
        e.xclient.data.l[2] = 0;
        e.xclient.data.l[3] = 0;
        e.xclient.data.l[4] = 0;
        XSendEvent( dpy, xwa.root, false, SubstructureNotifyMask | SubstructureRedirectMask, &e );
    }
    if(currentAPI == API_SDL2)
    {
        if(fullscreen)
            SDL_SetWindowFullscreen(window,SDL_GetWindowFlags(window) | SDL_WINDOW_FULLSCREEN);
        else
            SDL_SetWindowFullscreen(window,SDL_GetWindowFlags(window) & ~SDL_WINDOW_FULLSCREEN);
    }
}
bool pxgEngine::KeyDown(unsigned char key)
{
    if(currentAPI!=API_X11)
        return false;
    return keys[key];
}

bool pxgEngine::KeyPressed(unsigned char key)
{
    if(currentAPI!=API_X11)
        return false;
    return pressed_keys[key];
}

int pxgEngine::MouseX()
{
    if(currentAPI == API_X11)
    {
        XCheckWindowEvent(dpy,win,PointerMotionMask,&xev);
        return xev.xmotion.x;
    }
    if(currentAPI == API_SDL2)
    {
        return Mx;
    }
}

int pxgEngine::MouseY()
{
    if(currentAPI == API_X11)
    {
        XCheckWindowEvent(dpy,win,PointerMotionMask,&xev);
        return xev.xmotion.y;
    }
    if(currentAPI == API_SDL2)
    {
        return My;
    }
}

bool pxgEngine::MouseLB()
{
    if(currentAPI != API_SDL2)
        return false;
    return mouseState & SDL_BUTTON_LMASK;
}

bool pxgEngine::MouseMB()
{
    if(currentAPI != API_SDL2)
        return false;
    return mouseState & SDL_BUTTON_MMASK;
}

bool pxgEngine::MouseRB()
{
    if(currentAPI != API_SDL2)
        return false;
    return mouseState & SDL_BUTTON_RMASK;
}

bool pxgEngine::MouseLBClick()
{
    return sdl_lmb_press;
}

bool pxgEngine::MouseRBClick()
{
    return sdl_rmb_press;
}

bool pxgEngine::MouseMBClick()
{
    return sdl_mmb_press;
}

int pxgEngine::MouseWheel()
{
    return wheel;
}

void pxgEngine::DrawHUD()
{
    if(hudManager==NULL)
        return;
    hudManager->Render();
}

int pxgEngine::GetTime()
{
    timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec/1000;
}

void pxgEngine::SetCursorPos(int x, int y)
{
    if(currentAPI == API_X11)
        XWarpPointer(dpy,None,win,0,0,0,0,x,y);
    if(currentAPI == API_SDL2)
        SDL_WarpMouseInWindow(window, x, y);
    return;
}


pxgTexturePool* pxgEngine::TexturePool()
{
    static pxgTexturePool* texPool = new pxgTexturePool();
    return texPool;
}
