
#ifndef _ZEQ_TRANSFORMABLE_HPP_
#define _ZEQ_TRANSFORMABLE_HPP_

#include "define.hpp"
#include "mat4.hpp"
#include "model_prototype.hpp"

class Transformable
{
private:
    zeq_model_proto_t*  m_prototype;
    Mat4                m_modelMatrix;
    Mat4*               m_parentMatrix;
    
public:
    Transformable(zeq_model_proto_t* proto);
    virtual ~Transformable();
    
    zeq_model_proto_t* getModelPrototype() { return m_prototype; }

    void setModelMatrix(Mat4& mat) { m_modelMatrix = mat; }
    Mat4& getModelMatrix() { return m_modelMatrix; }
};

#endif//_ZEQ_TRANSFORMABLE_HPP_
