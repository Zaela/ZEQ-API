
#include "model_static.hpp"

ModelStatic::ModelStatic(zeq_model_proto_t* proto)
: Transformable(proto)
{
    
}

ModelStatic::~ModelStatic()
{

}

void ModelStatic::calculateBoundingBox(AABB& box)
{
    zeq_model_proto_t* proto = getModelPrototype();
    
    if (!proto)
        return;
    
    Mat4 matrix = getModelMatrix();
    Vec3 pos;
    
    for (VertexBuffer* vb : proto->getVertexBuffers())
    {
        for (Vertex& vert : *vb)
        {
            matrix.transformVector(pos, vert.pos);
            box.addInternalPoint(pos);
        }
    }
    
    for (VertexBuffer* vb : proto->getVertexBuffersNoCollide())
    {
        for (Vertex& vert : *vb)
        {
            matrix.transformVector(pos, vert.pos);
            box.addInternalPoint(pos);
        }
    }
}

void ModelStatic::draw()
{
    // Assumes matrix mode is GL_MODELVIEW and that the camera view has been applied
    zeq_model_proto_t* proto = getModelPrototype();
    
    if (!proto)
        return;
    
    glPushMatrix();
    glMultMatrixf(getModelMatrix().ptr());
    
    zeq_blend_t lastBlend   = ZEQ_BLEND_INVISIBLE;
    uint32_t lastDiffuseId  = 0;
    
    for (VertexBuffer* vb : proto->getVertexBuffers())
    {
        if (vb->isInvisible())
            continue;
        
        vb->setActiveTexture(lastDiffuseId);
        
        if (vb->setActiveBlend(lastBlend))
        {
            if (lastBlend == ZEQ_BLEND_PARTICLE)
                Fog::disable();
            else
                Fog::enable();
        }
        
        vb->draw();
    }
    
    for (VertexBuffer* vb : proto->getVertexBuffersNoCollide())
    {
        if (vb->isInvisible())
            continue;
        
        vb->setActiveTexture(lastDiffuseId);
        
        if (vb->setActiveBlend(lastBlend))
        {
            if (lastBlend == ZEQ_BLEND_PARTICLE)
                Fog::disable();
            else
                Fog::enable();
        }
        
        vb->draw();
    }
    
    zeq_material_t::deactivateBlend(lastBlend);
    
    glPopMatrix();
}
