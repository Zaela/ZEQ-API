
#ifndef _ZEQ_EQG_MODEL_HPP_
#define _ZEQ_EQG_MODEL_HPP_

#include "define.hpp"
#include "conv_model.hpp"
#include "pfs.hpp"
#include "eqg_material.hpp"
#include "eqg_weight_buffer.hpp"
#include "vec3.hpp"
#include <vector>
#include <string>
#include <unordered_map>

class EqgModel : public ConvModel
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
        uint32_t typeNameIndex; // Index of the property's type name in the file's string block
        uint32_t valueType;     // Type of the property's value: 0 = float, 2 = string index int, 3 = ARGB color value int
        union {
            uint32_t    asIndex;
            float       asFloat;
            struct { uint8_t a, r, g, b; } asColor;
        };
    };
    
    struct EqgVertex
    {
        float x, y, z;  // Position
        float i, j, k;  // Normal
        float u, v;     // Texture coordinates
    };
    
    struct EqgVertexV3
    {
        float       x, y, z;    // Position
        float       i, j, k;    // Normal
        uint8_t     r, g, b, a;
        float       u, v;       // Texture coordinates
        float       unk_f[2];
    };
    
    struct EqgTriangle
    {
        uint32_t    index[3];
        int         materialIndex;
        uint32_t    flag;
    };
    
    struct EqgQuat
    {
        float x, y, z, w;
    };
    
    struct EqgBone
    {
        uint32_t    nameIndex;
        uint32_t    linkBoneIndex;
        uint32_t    flag;
        uint32_t    childBoneIndex;
        Vec3        pos;
        EqgQuat     rot;
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
        uint32_t    boneAssignmentCount;
    };

private:
    typedef void (*PropertyHandler)(EqgMaterial* mat, EqgProperty& prop, EqgModel* model);
    std::unordered_map<std::string, PropertyHandler>    m_propertyHandlers;

    std::vector<EqgWeightBuffer> m_weightBuffers;
    std::vector<EqgWeightBuffer> m_weightBuffersNoCollide;

    byte*       m_data;
    uint32_t    m_len;
    uint32_t    m_version;
    uint32_t    m_stringBlockLen;
    const char* m_stringBlock;

private:
    void setVersion(uint32_t ver) { m_version = ver; }
    void setStringBlockLength(uint32_t len) { m_stringBlockLen = len; }
    void setStringBlock(const char* strings) { m_stringBlock = strings; }
    
    inline void checkLength(uint32_t p)
    {
        if (p > m_len)
            throw 1; //fixme
    }
    
    uint32_t extractStrings(uint32_t p);
    uint32_t extractMaterials(uint32_t p);
    
    void readProperty(EqgMaterial* mat, EqgProperty& prop);
    
    // Property handlers
    static void handleDiffuse(EqgMaterial* mat, EqgProperty& prop, EqgModel* model);
    
public:
    EqgModel(PFS* pfs, byte* data, uint32_t len);

    void initWeightBuffers();

    void loadCommon();
    void loadMDS();

    uint32_t version() const { return m_version; }
    uint32_t stringsLength() const { return m_stringBlockLen; }
    const char* strings() { return m_stringBlock; }
    
    const char* getString(uint32_t index);
};

#endif//_ZEQ_EQG_MODEL_HPP_
