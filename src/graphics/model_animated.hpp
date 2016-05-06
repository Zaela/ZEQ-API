
#ifndef _ZEQ_MODEL_ANIMATED_HPP_
#define _ZEQ_MODEL_ANIMATED_HPP_

#include "define.hpp"
#include "model_prototype.hpp"
#include "vertex_buffer.hpp"
#include "transformable.hpp"
#include "axis_aligned_bounding_box.hpp"
#include "fog.hpp"
#include "skeleton.hpp"
#include "animation.hpp"
#include <vector>

class ModelAnimated : public Transformable
{
private:
    struct VertexBufferSet
    {
        uint32_t        vertCount;
        VertexBuffer*   src;
        VertexBuffer*   dst;
    };
    
    struct WeightedVertexBufferSet : public VertexBufferSet
    {
        uint32_t weightCount;
    };
    
    struct SimpleHead
    {
        uint32_t count;
        union
        {
            VertexBufferSet     single;
            VertexBufferSet*    array;
        };
    };
    
    struct WeightedHead
    {
        uint32_t count;
        union
        {
            WeightedVertexBufferSet     single;
            WeightedVertexBufferSet*    array;
        };
    };
    
    struct AnimData
    {
        Vec3        pos;
        Quaternion  rot;
        Vec3        scale;
        uint32_t    frameHint;
    };

private:
    bool            m_isEqg;
    Skeleton        m_skeleton;
    AnimationSet    m_animations;
    Mat4*           m_animMatrices;

    // Active animation info
    int         m_curAnimId; 
    Animation*  m_curAnim;
    float       m_curAnimDuration;
    float       m_curAnimFrame;

    union
    {
        SimpleHead*     m_curHeadSimple;
        WeightedHead*   m_curHeadWeighted;
    };

    union
    {
        std::vector<VertexBufferSet>            m_simpleVBs;
        std::vector<WeightedVertexBufferSet>    m_weightedVBs;
    };
    
    union
    {
        std::vector<SimpleHead>     m_simpleHeads;
        std::vector<WeightedHead>   m_weightedHeads;
    };
    
private:
    void addVertexBuffersSimple(std::vector<VertexBuffer*>& vbs);

    // Animation
    void buildMatrices();
    AABB moveVerticesEQG();
    AABB moveVerticesWLD();
    float incrementAnimation(float delta);
    
public:
    ModelAnimated(zeq_model_proto_t* proto);
    virtual ~ModelAnimated();
    
    void draw();

    void setAnimation(int animId);
    bool animate(float delta, AABB& box);
    void assumeRagdollPosition();
};

#endif//_ZEQ_MODEL_ANIMATED_HPP_
