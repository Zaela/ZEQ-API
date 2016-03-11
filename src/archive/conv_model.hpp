
#ifndef _ZEQ_CONV_MODEL_HPP_
#define _ZEQ_CONV_MODEL_HPP_

#include "define.hpp"
#include "conv_object.hpp"
#include "conv_material.hpp"
#include "conv_vertex_buffer.hpp"
#include "pfs.hpp"
#include "mat4.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class ConvModel : public ConvObject
{
private:
    struct ObjectPlacement
    {
        ConvModel*  model;
        Mat4        transform;
    };
    
private:
    PFS* m_pfs;

    std::vector<ConvMaterial*>      m_materials; //may be able to flatten this?
    std::vector<ConvVertexBuffer>   m_vertexBuffers;
    std::vector<ConvVertexBuffer>   m_noCollideVertexBuffers;
    std::vector<ConvModel*>         m_headModels;
//skele, anims

    std::unordered_map<std::string, ConvModel*> m_objectModelsByName;
    std::vector<ObjectPlacement>                m_objectPlacementsStatic;
    std::vector<ObjectPlacement>                m_objectPlacementsAnimated;

private:
    void takeVertexBuffers(std::vector<ConvVertexBuffer>& src, std::vector<ConvVertexBuffer>& dst);

public:
    ConvModel(PFS* pfs);
    virtual ~ConvModel();
    
    PFS* getPFS() const { return m_pfs; }
    
    void addMaterial(ConvMaterial* mat) { m_materials.push_back(mat); }
    ConvMaterial* getMaterial(uint32_t i) { return m_materials[i]; }
    std::vector<ConvMaterial*>& getAllMaterials() { return m_materials; }
    uint32_t getMaterialCount() const { return m_materials.size(); }
    
    ConvVertexBuffer* getVertexBuffer(uint32_t index);
    ConvVertexBuffer* getVertexBufferNoCollide(uint32_t index);
    
    void initVertexBuffers();
    
    void prepareTextures(zeq_t* Z);
    
    void addObjectsAsStaticGeometry(ConvModel& objModels);
    
    std::vector<ConvVertexBuffer>& getVertexBuffers() { return m_vertexBuffers; }
    std::vector<ConvVertexBuffer>& getVertexBuffersNoCollide() { return m_noCollideVertexBuffers; }
    
    void takeVertexBuffers(std::vector<ConvVertexBuffer>& vbs);
    void takeVertexBuffersNoCollide(std::vector<ConvVertexBuffer>& vbs);
    
    bool isAnimated() const;
    
    void addObjectDefinition(const char* name, ConvModel* obj);
    ConvModel* getObjectDefinition(const char* name);
    
    void addObjectPlacement(Mat4& transform, ConvModel* obj);
};

#endif//_ZEQ_CONV_MODEL_HPP_