
#ifndef _ZEQ_ZONE_MODEL_HPP_
#define _ZEQ_ZONE_MODEL_HPP_

#include "define.hpp"
#include "zeq_object.hpp"
#include "vertex_buffer.hpp"
#include "conv_model.hpp"
#include "conv_vertex_buffer.hpp"
#include "axis_aligned_bounding_box.hpp"
#include "mat4.hpp"
#include "vec3.hpp"
#include "camera.hpp"
#include "model_prototype.hpp"
#include "model_instance.hpp"
#include <string>
#include <vector>
#include <unordered_set>

#define ZEQ_ZONE_SMALL_OCTREE_NODE_MAX_RADIUS (250.0f * 250.0f)

class zeq_zone_model_t : public ZeqObject
{
private:
    struct BoundingBox
    {
        bool collideable;
        AABB box;
    };
    
    struct ObjectSet
    {
        // probably worth splitting placed instances into their individual vertex buffers, ordered by texture;
        // worth the cost of having more bounding boxes to check against in return for switching textures less
        // in densely populated zones
        std::vector<AABB>               boundingBoxes;
        std::vector<zeq_model_inst_t*>  placements;
    };
    
private:
    std::string m_name;
    bool        m_registeredWithOpenGL;

    std::vector<BoundingBox>    m_boundingBoxes;
    std::vector<VertexBuffer*>  m_vertexBuffers;

    ObjectSet m_staticObjects;
    ObjectSet m_animatedObjects;

private:
    // Structs to hold temp data during octree generation
    struct OctreeTempSet
    {
        std::vector<AABB>               boundingBoxes;
        std::vector<ConvVertexBuffer>   vertexBuffers;
    };
    
    struct OctreeTemp
    {
        std::vector<uint32_t>   claimed;
        OctreeTempSet           sets[(int)ZEQ_BLEND_COUNT];
        
        void placeVertexBuffer(ConvVertexBuffer& vb, AABB& box)
        {
            int i = (int)vb.getMaterial()->getBlend();
            sets[i].boundingBoxes.push_back(box);
            sets[i].vertexBuffers.push_back(std::move(vb));
        }
    };

private:
    void generateOctree(ConvModel& zone, ConvModel& objects, uint32_t maxTrianglesPerNode = 512, int useStaticGeometryForObjects = 1);
    void generateOctreeFrom(std::vector<ConvVertexBuffer>& vbs, uint32_t maxTrianglesPerNode, bool collideable);
    void addPlacedObjects(ConvModel& objects, int useStaticGeometryForObjects = 1);

    static void octreeRecurse(OctreeTemp& temp, ConvVertexBuffer& vb, Triangle* triArray, uint32_t vbTriCount,
        AABB& parentBox, uint32_t parentId, uint32_t parentTriCount, uint32_t& curClaim, uint32_t maxTrianglesPerNode);
    static void octreeMakeNode(OctreeTemp& temp, ConvVertexBuffer& vb, Triangle* triArray, uint32_t triCount, uint32_t claim);
    static bool octreeCheckSmallVertexBuffer(ConvVertexBuffer& vb, OctreeTemp& temp);

    void registerWithOpenGL();
    
public:
    zeq_zone_model_t(zeq_t* Z, const std::string& name, ConvModel& zone, ConvModel& objects,
        uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects);
    ~zeq_zone_model_t();

    void draw(zeq_camera_t* cam);
};

#endif//_ZEQ_ZONE_MODEL_HPP_
