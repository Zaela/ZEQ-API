
#ifndef _ZEQ_EQG_MODEL_HPP_
#define _ZEQ_EQG_MODEL_HPP_

#include "define.hpp"
#include "conv_model.hpp"
#include "pfs.hpp"
#include "eqg_material.hpp"
#include "eqg_weight_buffer.hpp"
#include "eqg_common.hpp"
#include "eqg_ani.hpp"
#include "vec3.hpp"
#include <vector>
#include <string>
#include <unordered_map>

class EqgModel : public ConvModel, public EqgCommon
{
private:
#pragma pack(1)
    struct EqgBinMaterial
    {
        uint32_t    index;          // Essentially meaningless
        uint32_t    nameIndex;      // Index of the material's name in the file's string block
        uint32_t    shaderIndex;    // Index of the name of the shader to use for this material in the file's string block
        uint32_t    propertyCount;  // Number of EQGProperty elements following this material
    };
    
    struct EqgProperty
    {
        enum Type : uint32_t
        {
            Float = 0,
            Index = 2,
            Color = 3
        };
        
        uint32_t    typeNameIndex; // Index of the property's type name in the file's string block
        Type        valueType;     // Type of the property's value: 0 = float, 2 = string index int, 3 = ARGB color value int
        union {
            uint32_t    asIndex;
            float       asFloat;
            struct { uint8_t a, r, g, b; } asColor;
        };
    };
    
    struct EqgVertex
    {
        Vec3 pos;
        Vec3 normal;
        float u, v;
    };
    
    struct EqgVertexV3
    {
        Vec3        pos;
        Vec3        normal;
        uint8_t     r, g, b, a;
        float       u, v;
        float       unk_f[2];
    };
    
    struct EqgTriangle
    {
        uint32_t    index[3];
        int         materialIndex;
        uint32_t    flag;
    };
    
    struct EqgBone
    {
        uint32_t    nameIndex;
        uint32_t    linkBoneIndex;
        uint32_t    flag;
        uint32_t    childBoneIndex;
        Vec3        pos;
        Quaternion  rot;
        Vec3        scale;
    };
    
    struct EqgWeight
    {
        int     boneIndex;
        float   value;
    };
    
    struct EqgBoneAssignment
    {
        uint32_t    count;
        EqgWeight   weights[4];
    };
#pragma pack()
    
    struct MdsHeader
    {
        uint32_t    signature;      // "EQGS"
        uint32_t    version;
        uint32_t    stringBlockLen;
        uint32_t    materialCount;
        uint32_t    boneCount;
        uint32_t    modelCount;
    };
    
    struct MdsModelHeader
    {
        uint32_t    mainPiece;      // 1 = yes, 0 = no; yes applies to both the main body and default "head"
        uint32_t    nameIndex;
        uint32_t    vertexCount;
        uint32_t    triangleCount;
        uint32_t    boneAssignmentCount;    // always equal to vertexCount
    };
    
    struct MdsHeadTemp
    {
        uint32_t    headNumber;
        EqgModel*   model;
    };

private:
    typedef void (*PropertyHandler)(EqgMaterial* mat, EqgProperty* prop, EqgModel* model);
    std::unordered_map<std::string, PropertyHandler>    m_propertyHandlers;

    std::vector<EqgWeightBuffer> m_weightBuffers;
    std::vector<EqgWeightBuffer> m_weightBuffersNoCollide;

    bool            m_ownsData;
    EqgTriangle*    m_triangles;

private:
    AttachPoint::Type getAttachPointType(const std::string& boneName);
    
    uint32_t extractMaterials(uint32_t p, uint32_t count);
    uint32_t extractBones(uint32_t p, uint32_t count);
    uint32_t extractVertexBuffers(uint32_t p, uint32_t vertCount, uint32_t triCount, bool isZone = false);
    uint32_t extractBoneAssignments(uint32_t p, uint32_t count, uint32_t triCount, std::unordered_map<uint32_t, uint32_t>& indexMap);
    void     extractAnimations();
    
    EqgModel* createHeadModel();
    
    void readProperty(EqgMaterial* mat, EqgProperty* prop);
    
    // Property handlers
    static void handleDiffuse(EqgMaterial* mat, EqgProperty* prop, EqgModel* model);
    
public:
    EqgModel(PFS* pfs, byte* data, uint32_t len, bool ownsData = true);
    virtual ~EqgModel();

    void initWeightBuffers();

    void loadCommon();
    void loadMDS();
};

#endif//_ZEQ_EQG_MODEL_HPP_
