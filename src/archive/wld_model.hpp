
#ifndef _ZEQ_WLD_MODEL_HPP_
#define _ZEQ_WLD_MODEL_HPP_

#include "define.hpp"
#include "conv_model.hpp"
#include "wld_material.hpp"
#include "pfs.hpp"
#include "wld.hpp"
#include "fragments.hpp"
#include <unordered_map>
#include <vector>
#include <functional>

class WldModel : public ConvModel
{
private:
    struct ObjectPlacement
    {
        WldModel*   model;
        float       x, y, z;
        float       rotY, rotZ;
        float       scale;
    };
    
private:
    WLD* m_wld;

    std::unordered_map<Fragment*, uint32_t> m_materialIndicesByF30;

private:
#pragma pack(1)
    struct WldVertex
    {
        int16_t x, y, z;
    };
    
    struct WldUv16
    {
        int16_t u, v;
    };
    
    struct WldUv32
    {
        float u, v;
    };
    
    struct WldNormal
    {
        int8_t i, j, k;
    };
    
    struct WldColor
    {
        uint8_t r, g, b, a;
    };
    
    struct WldTriangle
    {
        uint16_t flag;
        uint16_t index[3];
    };
    
    struct WldTextureEntry
    {
        uint16_t count;
        uint16_t index;
    };
    
    struct WldBoneAssignment
    {
        uint16_t count;
        uint16_t index;
    };
#pragma pack()

private:
    void handleF03(Frag03* f03, Frag30* f30, ConvMaterial* mat = nullptr);
    void handleF04Animated(Frag04* f04s, Frag30* f30);
    void readMaterials(Frag14* f14);
    void readMesh(Frag36* f36, bool isZone = false, ConvSkeleton* skele = nullptr);
    void readAnimatedModel(Frag11* f11);

public:
    WldModel(PFS* pfs, WLD* wld);

    void readAllMaterials();
    void readAllMeshes();
    void readObjectDefinitions();
    void readObjectPlacements(WLD* placementWld);
    void readModel(const char* modelId);
};

#endif//_ZEQ_WLD_MODEL_HPP_
