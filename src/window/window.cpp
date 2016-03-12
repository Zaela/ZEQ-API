
#include "window.hpp"

zeq_window_t::zeq_window_t(zeq_t* Z, const char* title, uint32_t width, uint32_t height, int options)
: m_zeq(Z)
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
    
    if (options & ZEQ_WINDOW_RESIZABLE)
    {
        style |= sf::Style::Resize;
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

bool zeq_window_t::pollInput(zeq_input_t* input)
{
    sf::Event ev;
    
    if (pollEvent(ev))
    {
        memcpy(input, &ev, sizeof(zeq_input_t));
        
        zeq_input_event_t t;
        
        switch (ev.type)
        {
        case sf::Event::Closed:
            t = ZEQ_INPUT_WINDOW_CLOSE;
            break;
        case sf::Event::Resized:
            t = ZEQ_INPUT_WINDOW_RESIZE;
            break;
        case sf::Event::LostFocus:
            t = ZEQ_INPUT_WINDOW_LOSE_FOCUS;
            break;
        case sf::Event::GainedFocus:
            t = ZEQ_INPUT_WINDOW_GAIN_FOCUS;
            break;
        case sf::Event::TextEntered:
            t = ZEQ_INPUT_TEXT_ENTRY;
            break;
        case sf::Event::KeyPressed:
            t = ZEQ_INPUT_KEY_PRESS;
            break;
        case sf::Event::KeyReleased:
            t = ZEQ_INPUT_KEY_RELEASE;
            break;
        case sf::Event::MouseWheelScrolled:
            t = ZEQ_INPUT_MOUSE_WHEEL;
            break;
        case sf::Event::MouseButtonPressed:
            t = ZEQ_INPUT_MOUSE_CLICK;
            break;
        case sf::Event::MouseButtonReleased:
            t = ZEQ_INPUT_MOUSE_RELEASE;
            break;
        case sf::Event::MouseMoved:
            t = ZEQ_INPUT_MOUSE_MOVE;
            break;
        case sf::Event::MouseEntered:
            t = ZEQ_INPUT_MOUSE_ENTER_WINDOW;
            break;
        case sf::Event::MouseLeft:
            t = ZEQ_INPUT_MOUSE_LEAVE_WINDOW;
            break;
        default:
            t = ZEQ_INPUT_UNKNOWN;
            break;
        }
        
        input->event = t;
        return true;
    }
    
    return false;
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

void zeq_window_change_viewport(uint32_t width, uint32_t height)
{
    glViewport(0, 0, width, height);
}

void zeq_window_resize(zeq_window_t* window, uint32_t width, uint32_t height)
{
    sf::Vector2u v(width, height);
    window->setSize(v);
}

void zeq_window_clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void zeq_window_display(zeq_window_t* win)
{
    win->display();
}

int zeq_window_poll_input(zeq_window_t* window, zeq_input_t* input)
{
    if (!input)
        return 0;
    
    return (int)window->pollInput(input);
}
