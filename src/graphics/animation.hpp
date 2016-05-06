
#ifndef _ZEQ_ANIMATION_HPP_
#define _ZEQ_ANIMATION_HPP_

#include "define.hpp"
#include "ref_counter.hpp"
#include "fragments.hpp"
#include "vec3.hpp"
#include "quaternion.hpp"
#include <vector>

class Animation : public RefCounter
{
private:
    struct Frame
    {
        float       milliseconds;
        Vec3        pos;
        Quaternion  rot;
        Vec3        scale;
    };
    
public:
    static const uint32_t DEFAULT_HINT = 1;
    
private:
    uint32_t            m_boneCount;
    float               m_durationMs;
    float               m_frameRate;
    uint32_t            m_frameCount;
    std::vector<Frame>  m_framesByBoneIndex[1];

private:
    Animation(uint32_t frameCount, uint32_t frameRate, uint32_t highBoneIndex);

    void add(std::vector<Frame>& frames, uint32_t i, Frag12Entry& ent);
    
public:
    static Animation* create(uint32_t frameCount, uint32_t frameRate, uint32_t highBoneIndex);
    virtual ~Animation();

    void addFrames(uint32_t boneIndex, Frag12* f12);

    float   getDurationMilliseconds() const { return m_durationMs; }
    void    getFrameData(float frame, uint32_t boneIndex, Vec3& pos, Quaternion& rot, Vec3& scale, uint32_t& hint);
    void    getFrameData(float frame, uint32_t boneIndex, Vec3& pos, Vec3& rot, uint32_t& hint);
};


class AnimationSet
{
private:
    struct AnimationPlusId
    {
        int         id;
        Animation*  anim;
    };
    
private:
    std::vector<AnimationPlusId> m_animations;
    
public:
    AnimationSet();
    AnimationSet(const AnimationSet& o);
    AnimationSet(AnimationSet&& o);
    ~AnimationSet();

    void addAnimation(int animId, Animation* anim);

    Animation* getAnimation(int animId);
};

#endif//_ZEQ_ANIMATION_HPP_
