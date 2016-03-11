
#include "window.hpp"

zeq_window_t::zeq_window_t(zeq_t* Z, const char* title, uint32_t width, uint32_t height, int options)
: m_zeq(Z),
  m_running(true)
{
    sf::ContextSettings settings;
    settings.depthBits          = 24;
    settings.stencilBits        = 8;
    settings.antialiasingLevel  = 0;
    settings.majorVersion       = 3;
    settings.minorVersion       = 0;
    
    int style = 0;
    
    if (!(options & ZEQ_WINDOW_HIDE_TITLEBAR))
    {
        style = sf::Style::Titlebar | sf::Style::Close;
    }
    
    if (options & ZEQ_WINDOW_FULLSCREEN)
    {
        style |= sf::Style::Fullscreen;
    }
    
    sf::VideoMode videoMode;
    
    if (options & ZEQ_WINDOW_USE_NATIVE_ASPECT_RATIO)
    {
        videoMode = sf::VideoMode::getDesktopMode();
    }
    else
    {
        videoMode.width     = width ? width : 800;
        videoMode.height    = height ? height : 600;
    }
    
    create(videoMode, title ? title : "ZEQ", style, settings);
    
    setKeyRepeatEnabled(false);
    setVerticalSyncEnabled((options & ZEQ_WINDOW_VSYNC) != 0);
    
    zeq_opengl_context_created(Z);
}

zeq_window_t::~zeq_window_t()
{
    close();
}

/*
** API functions
*/

zeq_window_t* zeq_window_create(zeq_t* Z, const char* title, uint32_t width, uint32_t height, int options)
{
    try
    {
        return new zeq_window_t(Z, title, width, height, options);
    }
    catch (...)
    {
        return nullptr;
    }
}

void zeq_window_destroy(zeq_window_t* win)
{
    delete win;
}

void zeq_window_clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void zeq_window_display(zeq_window_t* win)
{
    win->display();
}
