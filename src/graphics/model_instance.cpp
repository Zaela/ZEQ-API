
#include "model_instance.hpp"

zeq_model_inst_t::zeq_model_inst_t(zeq_model_proto_t* proto)
: m_isAnimated(proto->isAnimated()),
  m_isWld(false)
{
    if (!m_isAnimated)
    {
        new (&m_static) ModelStatic(proto);
    }
}

zeq_model_inst_t::~zeq_model_inst_t()
{
    if (m_isAnimated)
    {
        m_static.~ModelStatic();
    }
}
