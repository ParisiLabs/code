#include <SDL.h>
#include <SDL_opengl.h>

#include "inexor/engine/engine.hpp" // Needed bc of Cubescript and bc of initwarning.
#include "inexor/rendering/windowmanagment.hpp"
#include "inexor/util/Logging.hpp"

// hardcoded macros for minimal/maximal screen settings

namespace inexor {
namespace rendering {
namespace screen {

/// screen resolution management
int screenw = 0, screenh = 0, desktopw = 0, desktoph = 0;

/// Simple DirectMedia Window and Layer
SDL_Window *sdl_window = NULL;
SDL_GLContext glcontext = NULL;

void cleanupSDL()
{
    if(SDL_WasInit(SDL_INIT_VIDEO))
    {
        // free SDL context
        if(sdl_window) SDL_SetWindowGrab(sdl_window, SDL_FALSE);
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_ShowCursor(SDL_TRUE);
        cleargamma();
    }
}

void screenres(int w, int h);

ICOMMAND(screenres, "ii", (int *w, int *h), screenres(*w, *h));

/// change screen width and height
VARF(scr_w, SCR_MINW, -1, SCR_MAXW, screenres(scr_w, -1));
VARF(scr_h, SCR_MINH, -1, SCR_MAXH, screenres(-1, scr_h));

/// various buffer precisions and anti aliasing
/// @see initwarning
VAR(colorbits, 0, 0, 32);
VARF(depthbits, 0, 0, 32, initwarning("depth-buffer precision"));
VARF(stencilbits, 0, 0, 32, initwarning("stencil-buffer precision"));
VARF(fsaa, -1, -1, 16, initwarning("anti-aliasing"));

VARFP(vsync, 0, 0, 1, restorevsync());
VARFP(vsynctear, 0, 0, 1, {if(vsync) restorevsync();});

/// set the swap interval for the current OpenGL context
void restorevsync()
{
    if(glcontext) SDL_GL_SetSwapInterval(vsync ? (vsynctear ? -1 : 1) : 0);
}

bool initwindowpos = false;

/// switch fullscreen mode
void setfullscreen(bool enable)
{
    if(!sdl_window) return;
    SDL_SetWindowFullscreen(sdl_window, enable ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    if(!enable)
    {
        SDL_SetWindowSize(sdl_window, scr_w, scr_h);
        if(initwindowpos)
        {
            SDL_SetWindowPosition(sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            initwindowpos = false;
        }
    }
}

/// @warning do not go full screen in debug mode (doesn't work with MSVC)
#ifdef _DEBUG
VARF(fullscreen, 0, 0, 1, setfullscreen(fullscreen!=0));
#else
VARF(fullscreen, 0, 1, 1, setfullscreen(fullscreen!=0));
#endif

/// change the screen resolution
void screenres(int w, int h)
{
    scr_w = w!=-1 ? clamp(w, SCR_MINW, SCR_MAXW) : scr_w;
    scr_h = h!=-1 ? clamp(h, SCR_MINH, SCR_MAXH) : scr_h;
    if(sdl_window)
    {
        scr_w = min(scr_w, desktopw);
        scr_h = min(scr_h, desktoph);
        if(SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_FULLSCREEN) gl_resize();
        else SDL_SetWindowSize(sdl_window, scr_w, scr_h);
    } else
    {
        initwarning("screen resolution");
    }
}

/// screen gamma as float value
static int curgamma = 100;
VARFP(gamma, 30, 100, 300,
{
    if(gamma == curgamma) return;
    curgamma = gamma;
    if(SDL_SetWindowBrightness(sdl_window, gamma/100.0f)==-1)
    spdlog::get("global")->error() << "Could not set gamma: " << SDL_GetError();
});

/// set screen brightness using float value
/// @see curgamma
void restoregamma()
{
    if(curgamma == 100) return;
    SDL_SetWindowBrightness(sdl_window, curgamma/100.0f);
}

/// set screen to normal brightness
void cleargamma()
{
    if(curgamma != 100 && sdl_window)
    {
        /// "Use this function to set the brightness (gamma multiplier) for the display that owns a given window."
        SDL_SetWindowBrightness(sdl_window, 1.0f);
    }
}

/// setting up screen using various attempts with different options
/// @see SDL_GL_SetAttribute
void setupscreen(int &useddepthbits, int &usedfsaa)
{
    if(glcontext)
    {
        SDL_GL_DeleteContext(glcontext);
        glcontext = NULL;
    }
    if(sdl_window)
    {
        SDL_DestroyWindow(sdl_window);
        sdl_window = NULL;
    }

    SDL_DisplayMode desktop;
    if(SDL_GetDesktopDisplayMode(0, &desktop) < 0) fatal("failed querying desktop display mode: %s", SDL_GetError());
    desktopw = desktop.w;
    desktoph = desktop.h;

    if(scr_h < 0) scr_h = SCR_DEFAULTH;
    if(scr_w < 0) scr_w = (scr_h*desktopw)/desktoph;
    scr_w = min(scr_w, desktopw);
    scr_h = min(scr_h, desktoph);

    int winw = scr_w, winh = scr_h, flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    if(fullscreen)
    {
        winw = desktopw;
        winh = desktoph;
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        initwindowpos = true;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    static int configs[] =
    {
        0x7, /* try everything */
        0x6, 0x5, 0x3, /* try disabling one at a time */
        0x4, 0x2, 0x1, /* try disabling two at a time */
        0 /* try disabling everything */
    };
    int config = 0;
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    if(!depthbits) SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    if(!fsaa)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
    }
    loopi(sizeof(configs)/sizeof(configs[0]))
    {
        config = configs[i];
        if(!depthbits && config&1) continue;
        if(!stencilbits && config&2) continue;
        if(fsaa<=0 && config&4) continue;
        if(depthbits) SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, config&1 ? depthbits : 16);
        if(stencilbits)
        {
            hasstencil = config&2 ? stencilbits : 0;
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, hasstencil);
        } else hasstencil = 0;
        if(fsaa>0)
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, config&4 ? 1 : 0);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, config&4 ? fsaa : 0);
        }
        sdl_window = SDL_CreateWindow("Inexor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winw, winh, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | flags);
        if(sdl_window) break;
    }
    if(!sdl_window) fatal("failed to create OpenGL window: %s", SDL_GetError());
    else
    {
        if(depthbits && (config&1)==0) spdlog::get("global")->warn() << *depthbits << " bit z-buffer not supported - disabling";
        if(stencilbits && (config&2)==0) spdlog::get("global")->warn() << "Stencil buffer not supported - disabling";
        if(fsaa>0 && (config&4)==0) spdlog::get("global")->warn() << *fsaa << " anti-aliasing not supported - disabling";
    }

    SDL_SetWindowMinimumSize(sdl_window, SCR_MINW, SCR_MINH);
    SDL_SetWindowMaximumSize(sdl_window, SCR_MAXW, SCR_MAXH);

    glcontext = SDL_GL_CreateContext(sdl_window);
    if(!glcontext) fatal("failed to create OpenGL context: %s", SDL_GetError());

    SDL_GetWindowSize(sdl_window, &screenw, &screenh);

    useddepthbits = config&1 ? depthbits : 0;
    usedfsaa = config&4 ? fsaa : 0;
    restorevsync();
}


}}} // ns inexor::rendering::screen
