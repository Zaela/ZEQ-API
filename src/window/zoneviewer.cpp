
#include "zoneviewer.hpp"

zeq_zoneviewer_t::zeq_zoneviewer_t(zeq_camera_t* cam)
: m_camera(cam),
  m_move(0),
  m_turn(0),
  m_mouseLook(false),
  m_mouseLookMoved(false),
  m_farClip(cam->getFarClip())
{
    cam->grab();
}

zeq_zoneviewer_t::~zeq_zoneviewer_t()
{
    if (m_camera)
        m_camera->drop();
}

void zeq_zoneviewer_t::handleKeyPress(zeq_key_press_t& key)
{
    switch (key.key)
    {
    case ZEQ_KEY_UP:
        m_move = MOVE_FORWARD;
        break;
    case ZEQ_KEY_DOWN:
        m_move = MOVE_BACKWARD;
        break;
    case ZEQ_KEY_LEFT:
        m_turn = TURN_LEFT;
        break;
    case ZEQ_KEY_RIGHT:
        m_turn = TURN_RIGHT;
        break;
    default:
        break;
    }
}

void zeq_zoneviewer_t::handleKeyRelease(zeq_key_press_t& key)
{
    switch (key.key)
    {
    case ZEQ_KEY_UP:
        if (m_move == MOVE_FORWARD)
            m_move = 0;
        break;
    case ZEQ_KEY_DOWN:
        if (m_move == MOVE_BACKWARD)
            m_move = 0;
        break;
    case ZEQ_KEY_LEFT:
        if (m_turn == TURN_LEFT)
            m_turn = 0;
        break;
    case ZEQ_KEY_RIGHT:
        if (m_turn == TURN_RIGHT)
            m_turn = 0;
        break;
    case ZEQ_KEY_EQUAL:
        m_farClip += 10.0f;
        m_camera->setFarClip(m_farClip);
        break;
    case ZEQ_KEY_DASH:
        if (m_farClip > 50.0f)
        {
            m_farClip -= 10.0f;
            m_camera->setFarClip(m_farClip);
        }
        break;
    default:
        break;
    }
}

void zeq_zoneviewer_t::handleMouseMove(zeq_mouse_move_t& mouse)
{
    if (m_mouseLook)
    {
        m_mouseRel.x += mouse.x - m_mousePos.x;
        m_mouseRel.y += mouse.y - m_mousePos.y;
        m_mouseLookMoved = true;
    }
    
    m_mousePos.set(mouse.x, mouse.y);
}

void zeq_zoneviewer_t::handleMouseClick(zeq_mouse_click_t& mouse)
{
    switch (mouse.button)
    {
    case ZEQ_MOUSE_BUTTON_RIGHT:
        m_mouseLook = true;
        break;
    default:
        break;
    }
}

void zeq_zoneviewer_t::handleMouseRelease(zeq_mouse_click_t& mouse)
{
    switch (mouse.button)
    {
    case ZEQ_MOUSE_BUTTON_RIGHT:
        m_mouseLook = false;
        break;
    default:
        break;
    }
}

void zeq_zoneviewer_t::update(double delta)
{
    if (!m_move && !m_turn && (!m_mouseLook || !m_mouseLookMoved))
        return;
    
    Vec3& pos   = m_camera->getPosition();
    Vec3 target = m_camera->getTarget() - pos;
    
    Vec2 relativeRotation = target.horizontalAngle();
    
    relativeRotation.y += m_mouseRel.x;
    relativeRotation.x -= m_mouseRel.y;
    
    m_mouseRel.set(0.0f, 0.0f);
    m_mouseLookMoved = false;
    
    if (!m_mouseLook && m_turn)
        relativeRotation.y += delta * 100.0 * m_turn;
    
    target.set(0.0f, 0.0f, std::max(1.0f, pos.length()));
    
    // Avoid gimbal lock
    if (relativeRotation.x > 180.0f)
    {
        if (relativeRotation.x < 271.0f)
            relativeRotation.x = 271.0f;
    }
    else
    {
        if (relativeRotation.x > 89.0f)
            relativeRotation.x = 89.0f;
    }
    
    Mat4 mat;
    mat.identity();
    mat.setRotation(relativeRotation.x, relativeRotation.y, 0.0f);
    mat.transformVector(target);
    
    if (m_move)
    {
        Vec3 moveDir = target;
        moveDir.normalize();
        moveDir *= delta * 100.0f * m_move;
        pos -= moveDir;
    }
    
    if (m_mouseLook && m_turn)
    {
        Vec3 strafe = target.crossProduct(m_camera->getUpVector());
        strafe.normalize();
        strafe *= delta * 100.0f * -m_turn;
        pos -= strafe;
    }
    
    target += pos;
    
    m_camera->setTarget(target);
}

/*
** API functions
*/

zeq_zoneviewer_t* zeq_zoneviewer_create(zeq_camera_t* cam)
{
    try
    {
        return new zeq_zoneviewer_t(cam);
    }
    catch (...)
    {
        return nullptr;
    }
}

void zeq_zoneviewer_destroy(zeq_zoneviewer_t* zoneviewer)
{
    delete zoneviewer;
}

void zeq_zoneviewer_handle_input(zeq_zoneviewer_t* zoneviewer, zeq_input_t* input)
{
    switch (input->event)
    {
    case ZEQ_INPUT_MOUSE_MOVE:
        zoneviewer->handleMouseMove(input->mouseMove);
        break;
    case ZEQ_INPUT_MOUSE_CLICK:
        zoneviewer->handleMouseClick(input->mouseClick);
        break;
    case ZEQ_INPUT_MOUSE_RELEASE:
        zoneviewer->handleMouseRelease(input->mouseRelease);
        break;
    case ZEQ_INPUT_KEY_PRESS:
        zoneviewer->handleKeyPress(input->keyPress);
        break;
    case ZEQ_INPUT_KEY_RELEASE:
        zoneviewer->handleKeyRelease(input->keyRelease);
        break;
    default:
        break;
    }
}

void zeq_zoneviewer_update(zeq_zoneviewer_t* zoneviewer, zeq_delta_t delta)
{
    zoneviewer->update(delta.seconds);
}
