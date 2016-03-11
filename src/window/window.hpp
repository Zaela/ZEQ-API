
#ifndef _ZEQ_WINDOW_HPP_
#define _ZEQ_WINDOW_HPP_

#include "define_window.hpp"

struct zeq_window_t : public sf::RenderWindow
{
private:
    zeq_t*  m_zeq;
    bool    m_running;
    
public:
    zeq_window_t(zeq_t* Z, const char* title, uint32_t width, uint32_t height, int options);
    virtual ~zeq_window_t();

    bool pollInput(zeq_input_t* input);
};

#endif//_ZEQ_WINDOW_HPP_
