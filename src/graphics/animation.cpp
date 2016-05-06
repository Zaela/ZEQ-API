
#include "animation.hpp"

Animation* Animation::create(uint32_t frameCount, uint32_t frameRate, uint32_t highBoneIndex)
{
    highBoneIndex++;
    
    Animation* anim = (Animation*)(new byte[sizeof(Animation) - sizeof(std::vector<Frame>) + sizeof(std::vector<Frame>) * highBoneIndex]);
    
    // Need to call ctor to ensure base class ctor is called
    new (anim) Animation(frameCount, frameRate, highBoneIndex);
    
    return anim;
}

Animation::Animation(uint32_t frameCount, uint32_t frameRate, uint32_t highBoneIndex)
: m_boneCount(highBoneIndex),
  m_durationMs((frameCount * frameRate) * 0.001f),
  m_frameRate(frameRate * 0.001f),
  m_frameCount(frameCount)
{
    for (uint32_t i = 1; i < highBoneIndex; i++)
    {
        new (&m_framesByBoneIndex[i]) std::vector<Frame>;
    }
}

Animation::~Animation()
{
    for (uint32_t i = 1; i < m_boneCount; i++)
    {
        m_framesByBoneIndex[i].~vector<Frame>();
    }
}

void Animation::add(std::vector<Frame>& frames, uint32_t i, Frag12Entry& ent)
{
    Frame fr;
    
    fr.milliseconds = i * m_frameRate;
    ent.getPosRot(fr.pos, fr.rot);
    fr.scale.set(1.0f, 1.0f, 1.0f);
    
    frames.push_back(fr);
}

void Animation::addFrames(uint32_t boneIndex, Frag12* f12)
{
    std::vector<Frame>& frames  = m_framesByBoneIndex[boneIndex];
    uint32_t count              = f12->count;
    Frag12Entry* entries        = f12->entry;
    
    add(frames, 0, entries[0]);
    
    if (count > 2)
    {
        uint32_t prev   = 0;
        uint32_t n      = count - 2;
        
        for (uint32_t i = 1; i < n; i++)
        {
            Frag12Entry& a = entries[prev];
            Frag12Entry& b = entries[i];
            Frag12Entry& c = entries[i + 1];
            
            // If a = b = c, we can eliminate b
            if (a != b || b != c)
            {
                add(frames, i, b);
                prev = i;
            }
        }
    }
    
    // Add last frame
    if (count > 1)
        add(frames, count - 1, entries[count - 1]);
    
    // Is this bone's animation shorter than the overall animation? If so, need an extra ending frame to match up
    if (count < m_frameCount)
        add(frames, m_frameCount - 1, entries[count - 1]);
}

void Animation::getFrameData(float frame, uint32_t boneIndex, Vec3& pos, Quaternion& rot, Vec3& scale, uint32_t& hint)
{
    std::vector<Frame>& frames  = m_framesByBoneIndex[boneIndex];
    uint32_t index              = 0;
    
    for (uint32_t i = hint; i < frames.size(); i++)
    {
        Frame& fr = frames[i];
        
        if (fr.milliseconds >= frame)
        {
            index = i;
            break;
        }
        
        hint++;
    }
    
    // Caller should have reset the animation, if this happens they are beyond the total duration
    if (frames.size() <= hint)
        return;
    
    Frame& a = frames[index];
    Frame& b = frames[index - 1];
    
    float f1 = frame - a.milliseconds;
    float f2 = b.milliseconds - frame + f1;
    
    // Position
    pos = ((b.pos - a.pos) / f2) * f1 + a.pos;
    
    // Scale
    scale = ((b.scale - a.scale) / f2) * f1 + a.scale;
    
    // Rotation
    rot.slerp(a.rot, b.rot, f1 / f2);
}

void Animation::getFrameData(float frame, uint32_t boneIndex, Vec3& pos, Vec3& rot, uint32_t& hint)
{
    std::vector<Frame>& frames  = m_framesByBoneIndex[boneIndex];
    uint32_t index              = 0;
    
    for (uint32_t i = hint; i < frames.size(); i++)
    {
        Frame& fr = frames[i];
        
        if (fr.milliseconds >= frame)
        {
            index = i;
            break;
        }
        
        hint++;
    }
    
    // Caller should have reset the animation, if this happens they are beyond the total duration
    if (frames.size() <= hint)
        return;
    
    Frame& a = frames[index];
    Frame& b = frames[index - 1];
    
    float f1 = frame - a.milliseconds;
    float f2 = b.milliseconds - frame + f1;
    
    // Position
    pos = ((b.pos - a.pos) / f2) * f1 + a.pos;
    
    Vec3 ra(a.rot.x, a.rot.y, a.rot.z);
    Vec3 rb(b.rot.x, b.rot.y, b.rot.z);
    
    // Rotation
    rot = ((rb - ra) / f2) * f1 + ra;
}

AnimationSet::AnimationSet()
{
    
}

AnimationSet::AnimationSet(const AnimationSet& o)
{
    // Our array will always be empty when this is called, no need to check for duplicates
    for (auto& ai : o.m_animations)
    {
        ai.anim->grab();
        m_animations.push_back(ai);
    }
}

AnimationSet::AnimationSet(AnimationSet&& o)
{
    m_animations = std::move(o.m_animations);
}

AnimationSet::~AnimationSet()
{
    for (auto& ai : m_animations)
    {
        if (ai.anim)
        {
            ai.anim->drop();
            ai.anim = nullptr;
        }
    }
}

void AnimationSet::addAnimation(int animId, Animation* anim)
{
    anim->grab();
    
    // Check for duplicate to replace
    for (auto& ai : m_animations)
    {
        if (ai.id == animId)
        {
            ai.anim->drop();
            ai.anim = anim;
            return;
        }
    }
    
    AnimationPlusId ai;
    
    ai.id   = animId;
    ai.anim = anim;
    
    m_animations.push_back(ai);
}

Animation* AnimationSet::getAnimation(int animId)
{
    for (auto& ai : m_animations)
    {
        if (ai.id == animId)
            return ai.anim;
    }
    
    return nullptr;
}
