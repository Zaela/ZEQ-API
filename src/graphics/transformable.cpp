
#include "transformable.hpp"

Transformable::Transformable(zeq_model_proto_t* proto)
: m_prototype(proto),
  m_modelMatrix(Mat4()),
  m_parentMatrix(nullptr)
{
    proto->grab();
}

Transformable::~Transformable()
{
    if (m_prototype)
        m_prototype->drop();
}
