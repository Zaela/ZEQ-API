
#include "transformable.hpp"

Transformable::Transformable(zeq_model_proto_t* proto)
: m_prototype(proto),
  m_modelMatrix(Mat4()),
  m_parentMatrix(nullptr)
{
    proto->grab();
    
    switch (proto->getBaseTransform())
    {
    case zeq_model_proto_t::Wld:
        m_modelMatrix = m_modelMatrix.adjustForWLD();
        break;
    case zeq_model_proto_t::Eqg:
        m_modelMatrix = m_modelMatrix.adjustForEQG();
        break;
    default:
        break;
    }
}

Transformable::~Transformable()
{
    if (m_prototype)
        m_prototype->drop();
}
