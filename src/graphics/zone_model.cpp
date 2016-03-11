
#include "zone_model.hpp"

zeq_zone_model_t::zeq_zone_model_t(zeq_t* Z, const std::string& name, ConvModel& zone, ConvModel& objects,
    uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects)
: ZeqObject(Z),
  m_name(name),
  m_registeredWithOpenGL(false)
{
    generateOctree(zone, objects, maxTrianglesPerNode, useStaticGeometryForObjects);
    addPlacedObjects(objects, useStaticGeometryForObjects);
}

zeq_zone_model_t::~zeq_zone_model_t()
{
    for (VertexBuffer* vb : m_vertexBuffers)
    {
        VertexBuffer::destroy(vb);
    }
}

void zeq_zone_model_t::generateOctree(ConvModel& zone, ConvModel& objects, uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects)
{
    zone.prepareTextures(Z());
    objects.prepareTextures(Z());
    
    if (useStaticGeometryForObjects)
        zone.addObjectsAsStaticGeometry(objects);
    
    generateOctreeFrom(zone.getVertexBuffers(), maxTrianglesPerNode, true);
    generateOctreeFrom(zone.getVertexBuffersNoCollide(), maxTrianglesPerNode, false);
    
    // Handle separate zone objects
    if (!useStaticGeometryForObjects)
    {
        //objects
    }
}

void zeq_zone_model_t::generateOctreeFrom(std::vector<ConvVertexBuffer>& vbs, uint32_t maxTrianglesPerNode, bool collideable)
{
    OctreeTemp temp;
    
    for (ConvVertexBuffer& vb : vbs)
    {
        uint32_t triCount = vb.triangleCount();
        
        if (triCount <= maxTrianglesPerNode && octreeCheckSmallVertexBuffer(vb, temp))
            continue;
        
        AABB box;
        
        for (Vertex& vert : vb)
        {
            box.addInternalPoint(vert.pos);
        }
        
        uint32_t curClaim = 0;
        temp.claimed.reserve(triCount);
        memset(temp.claimed.data(), 0, sizeof(uint32_t) * triCount);
        
        octreeRecurse(temp, vb, (Triangle*)vb.data(), triCount, box, 0, triCount, curClaim, maxTrianglesPerNode);
        
        vb.clear();
    }
    
    for (int i = 0; i < (int)ZEQ_BLEND_COUNT; i++)
    {
        OctreeTempSet& set = temp.sets[i];
        
        for (AABB& box : set.boundingBoxes)
        {
            BoundingBox bb;
            bb.collideable  = collideable;
            bb.box          = box;
            m_boundingBoxes.push_back(bb);
        }
        
        for (ConvVertexBuffer& vb : set.vertexBuffers)
        {
            m_vertexBuffers.push_back(VertexBuffer::create(vb));
            vb.clear();
        }
    }
}

bool zeq_zone_model_t::octreeCheckSmallVertexBuffer(ConvVertexBuffer& vb, OctreeTemp& temp)
{
    AABB box;
    
    for (Vertex& vert : vb)
    {
        box.addInternalPoint(vert.pos);
    }
    
    if (box.getSphereRadiusSquared() > ZEQ_ZONE_SMALL_OCTREE_NODE_MAX_RADIUS)
        return false;
    
    temp.placeVertexBuffer(vb, box);
    return true;
}

void zeq_zone_model_t::octreeRecurse(OctreeTemp& temp, ConvVertexBuffer& vb, Triangle* triArray, uint32_t vbTriCount,
    AABB& parentBox, uint32_t parentId, uint32_t parentTriCount, uint32_t& curClaim, uint32_t maxTrianglesPerNode)
{
    Vec3 corners[8];
    Vec3 mid;
    
    parentBox.getAllCorners(corners, mid);
    
    uint32_t* claimed = temp.claimed.data();
    
    for (int c = 0; c < 8; c++)
    {
        uint32_t claim = ++curClaim;
        AABB box(mid, corners[c]);
        uint32_t triCount = 0;
        
        for (uint32_t i = 0; i < vbTriCount; i++)
        {
            if (claimed[i] != parentId)
                continue;
            
            Triangle& tri = triArray[i];
            
            for (int j = 0; j < 3; j++)
            {
                Vertex& vert = tri.points[j];
                if (!box.containsPoint(vert.pos))
                    goto skip;
            }
            
            claimed[i] = claim;
            triCount++;
            
        skip: ;
        }
        
        parentTriCount -= triCount;
        
        if (triCount > maxTrianglesPerNode)
        {
            octreeRecurse(temp, vb, triArray, vbTriCount, box, claim, triCount, curClaim, maxTrianglesPerNode);
        }
        else if (triCount > 0)
        {
            octreeMakeNode(temp, vb, triArray, vbTriCount, claim);
        }
    }
    
    // Check if parent box still has ownership of any triangles
    if (parentTriCount > 0)
    {
        octreeMakeNode(temp, vb, triArray, vbTriCount, parentId);
    }
}

void zeq_zone_model_t::octreeMakeNode(OctreeTemp& temp, ConvVertexBuffer& vb, Triangle* triArray, uint32_t triCount, uint32_t claim)
{
    AABB box;
    ConvVertexBuffer tempVerts;
    
    tempVerts.setMaterial(vb.getMaterial());
    
    uint32_t* claimed = temp.claimed.data();
    
    for (uint32_t i = 0; i < triCount; i++)
    {
        if (claimed[i] != claim)
            continue;
        
        Triangle& tri = triArray[i];
        
        for (int j = 0; j < 3; j++)
        {
            tempVerts.push_back(tri.points[j]);
        }
        
        for (int j = 0; j < 3; j++)
        {
            box.addInternalPoint(tri.points[j].pos);
        }
    }
    
    temp.placeVertexBuffer(tempVerts, box);
}

void zeq_zone_model_t::addPlacedObjects(ConvModel& objects, int useStaticGeometryForObjects)
{
    
}

void zeq_zone_model_t::draw(zeq_camera_t* cam)
{
    cam->recalculate();
    cam->applyView();
    
    Frustum& frustum    = cam->getFrustum();
    uint32_t n          = m_boundingBoxes.size();
    
    if (!m_registeredWithOpenGL)
        registerWithOpenGL();
    
    zeq_blend_t lastBlend   = ZEQ_BLEND_INVISIBLE;
    uint32_t lastDiffuseId  = 0;
    
    for (uint32_t i = 0; i < n; i++)
    {
        if (!frustum.contains(m_boundingBoxes[i].box))
            continue;
        
        VertexBuffer* vb = m_vertexBuffers[i];
        
        if (vb->isInvisible())
            continue;
        
        vb->setActiveTexture(lastDiffuseId);
        vb->setActiveBlend(lastBlend);
        
        vb->draw();
    }
    
    zeq_material_t::deactivateBlend(lastBlend);
}

void zeq_zone_model_t::registerWithOpenGL()
{
    for (VertexBuffer* vb : m_vertexBuffers)
    {
        vb->registerWithOpenGL(true);
    }
    
    m_registeredWithOpenGL = true;
}

/*
** API functions
*/

void zeq_zone_destroy(zeq_zone_model_t* zone)
{
    delete zone;
}

void zeq_zone_draw(zeq_zone_model_t* zone, zeq_camera_t* cam)
{
    zone->draw(cam);
}
