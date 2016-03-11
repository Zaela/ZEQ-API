
#include "wld_model.hpp"

WldModel::WldModel(PFS* pfs, WLD* wld)
: ConvModel(pfs),
  m_wld(wld)
{

}

void WldModel::readAllMaterials()
{
    WLD* wld        = m_wld;
    auto& frag30s   = wld->getFragsByType(0x30);
    
    if (frag30s.size() == 0)
        throw 1; //fixme
    
    // Materials
    uint32_t i = 1; // Zeroth material is the NULL material
    
    for (Fragment* frag : frag30s)
    {
        Frag30* f30 = (Frag30*)frag;
        addMaterial(new WldMaterial(wld->getFragName(f30), f30->visibilityFlag));
        m_materialIndicesByF30[f30] = i++;
    }
    
    // VertexBuffers
    initVertexBuffers();
    
    // Textures
    for (Fragment* frag : frag30s)
    {
        Frag30* f30 = (Frag30*)frag;
        Frag03* f03 = nullptr;
        
        if (f30->ref > 0)
        {
            Frag05* f05 = (Frag05*)wld->getFrag(f30->ref);
            if (!f05) continue;
            Frag04* f04 = (Frag04*)wld->getFrag(f05->ref);
            if (!f04) continue;
            
            if (!f04->isAnimated())
            {
                f03 = (Frag03*)wld->getFrag(f04->ref);
            }
            else
            {
                handleF04Animated(f04, f30);
                continue;
            }
        }
        else
        {
            f03 = (Frag03*)wld->getFrag(f30->ref);
        }
        
        handleF03(f03, f30);
    }
}

void WldModel::readMaterials(Frag14* f14)
{
    /*WLD* wld = m_wld;
    // f14 -> f11 -> f10 -> f2d -> f36 -> f31 -> f30
    
    Frag2d* f2d;
    Frag11* f11 = (Frag11*)wld->getFrag(f14->firstRef());
    
    if (!f11) return;
    
    if (f11->type() != 0x11)
    {
        f2d = (Frag2d*)f11;
    }
    else
    {
        Frag10* f10 = (Frag10*)wld->getFrag(f11->ref);
    }*/
}

void WldModel::handleF03(Frag03* f03, Frag30* f30, ConvMaterial* mat)
{
    if (!mat)
        mat = getMaterial(m_materialIndicesByF30[f30]);
    
    if (!f03 || f03->stringLen == 0)
    {
    invis:
        mat->setBlend(ZEQ_BLEND_INVISIBLE);
        return;
    }
    
    uint32_t len = 0;
    byte* data   = getPFS()->getEntry(f03->string, len);
    
    if (!data)
        goto invis;
    
    mat->addTexture(f03->string, data, len);
}

void WldModel::handleF04Animated(Frag04* f04s, Frag30* f30)
{
    Frag04Animated* f04 = (Frag04Animated*)f04s;
    ConvMaterial* mat   = getMaterial(m_materialIndicesByF30[f30]);
    
    mat->setAnimDelay(f04->milliseconds);
    
    int* refList    = f04->refList;
    int n           = f04->count;
    
    for (int i = 0; i < n; i++)
    {
        Frag03* f03 = (Frag03*)m_wld->getFrag(refList[i]);
        handleF03(f03, f30, mat);
    }
}

void WldModel::readAllMeshes()
{
    for (Fragment* frag : m_wld->getFragsByType(0x36))
    {
        readMesh((Frag36*)frag, true);
    }
}

void WldModel::readMesh(Frag36* f36, bool isZone)
{
    WLD* wld        = m_wld;
    uint32_t len    = f36->length();
    byte* data      = (byte*)f36;
    uint32_t p      = sizeof(Frag36);
    
    if (p > len) throw 1; //fixme
    
    const float scale = 1.0f / (float)(1 << f36->scale);
    
    float cx = f36->x;
    float cy = f36->y;
    float cz = f36->z;
    
    // Vertices
    WldVertex* verts = (WldVertex*)(data + p);
    p += sizeof(WldVertex) * f36->vertCount;
    
    if (p > len) throw 1; //fixme
    
    // Texture coordinates
    WldUv16* uv16 = nullptr;
    WldUv32* uv32 = nullptr;
    
    if (f36->uvCount > 0)
    {
        if (wld->version() == 1)
        {
            uv16 = (WldUv16*)(data + p);
            p += sizeof(WldUv16) * f36->uvCount;
        }
        else
        {
            uv32 = (WldUv32*)(data + p);
            p += sizeof(WldUv32) * f36->uvCount;
        }
    }
    
    if (p > len) throw 1; //fixme
    
    // Normals
    WldNormal* norms = (WldNormal*)(data + p);
    p += sizeof(WldNormal) * f36->vertCount;
    
    if (p > len) throw 1; //fixme
    
    // Vertex colors
    WldColor* colors = (WldColor*)(data + p);
    p += sizeof(WldColor) * f36->colorCount;
    (void)colors; // Not used (yet?)
    
    if (p > len) throw 1; //fixme
    
    // Triangles
    WldTriangle* tris = (WldTriangle*)(data + p);
    p += sizeof(WldTriangle) * f36->polyCount;
    
    if (p > len) throw 1; //fixme
    
    // Bone assignments
    uint32_t boneCount = f36->boneAssignCount;
    std::vector<uint32_t> vert2ba;
    std::function<uint32_t(uint32_t)> triBAs;
    
    if (boneCount > 0)
    {
        WldBoneAssignment* boneAssigns = (WldBoneAssignment*)(data + p);
        p += sizeof(WldBoneAssignment) * boneCount;
        
        if (p > len) throw 1; //fixme
        
        for (uint32_t i = 0; i < boneCount; i++)
        {
            WldBoneAssignment ba = boneAssigns[i];
            for (uint32_t j = 0; j < ba.count; j++)
            {
                vert2ba.push_back(ba.index);
            }
        }
        
        //auto& indexMap = skeleton()->getBoneIndexMap();
        
        triBAs = [&](uint32_t index)
        {
            //return indexMap[vert2ba[index]];
            return vert2ba[index];
        };
    }
    
    // Materials
    std::vector<uint32_t> matIndices;
    Frag31* f31 = (Frag31*)wld->getFrag(f36->materialListRef);
    
    if (!f31) throw 1; //fixme
    
    for (uint32_t i = 0; i < f31->refCount; i++)
    {
        Fragment* f30 = (Frag30*)wld->getFrag(f31->refList[i]);
        matIndices.push_back(m_materialIndicesByF30[f30]);
    }
    
    uint32_t n = f36->polyTextureCount;
    for (uint32_t m = 0; m < n; m++)
    {
        WldTextureEntry te = *(WldTextureEntry*)(data + p);
        p += sizeof(WldTextureEntry);
        
        if (p > len) throw 1; //fixme
        
        if (te.index >= matIndices.size())
            continue;
        
        uint32_t matIndex = matIndices[te.index];
        
        if (matIndex >= getMaterialCount())
            continue;
        
        ConvVertexBuffer* nvb = getVertexBuffer(matIndex);
        ConvVertexBuffer* cvb = getVertexBufferNoCollide(matIndex);
        
        for (uint32_t i = 0; i < te.count; i++)
        {
            WldTriangle tri         = tris[i];
            ConvVertexBuffer* vb    = (tri.flag & 0x10) ? cvb : nvb;
            
            if (!vb)
                continue;
            
            Vertex v;
            
            for (int j = 2; j >= 0; j--)
            {
                uint16_t index  = tri.index[j];
                
                WldVertex vert  = verts[index];
                WldNormal norm  = norms[index];
                
                v.pos.x     = cx + (float)vert.x * scale;
                
                v.normal.x  = (float)norm.i * (1.0f / 127.0f);
                
                if (isZone)
                {
                    v.pos.z     = cy + (float)vert.y * scale;
                    v.pos.y     = cz + (float)vert.z * scale;
                
                    v.normal.z  = (float)norm.j * (1.0f / 127.0f);
                    v.normal.y  = (float)norm.k * (1.0f / 127.0f);
                }
                else
                {
                    v.pos.y     = cy + (float)vert.y * scale;
                    v.pos.z     = cz + (float)vert.z * scale;
                
                    v.normal.y  = (float)norm.j * (1.0f / 127.0f);
                    v.normal.z  = (float)norm.k * (1.0f / 127.0f);
                }
                
                if (uv16)
                {
                    WldUv16 uv = uv16[index];
                    
                    v.u =   (float)uv.u * (1.0f / 256.0f);
                    v.v = -((float)uv.v * (1.0f / 256.0f));
                }
                else if (uv32)
                {
                    WldUv32 uv = uv32[index];
                    
                    v.u = uv.u;
                    v.v = uv.v;
                }
                else
                {
                    v.u = 0;
                    v.v = 0;
                }
                
                vb->push_back(v);
            }
        }
        
        // Advance tris ptr to the next set
        tris += te.count;
    }
}

void WldModel::readObjectDefinitions()
{
    WLD* wld                            = m_wld;
    PFS* pfs                            = getPFS();
    std::vector<ConvVertexBuffer>& vbs  = getVertexBuffers();
    std::vector<ConvVertexBuffer>& cvb  = getVertexBuffersNoCollide();
    
    for (Fragment* frag : wld->getFragsByType(0x14))
    {
        Frag14* f14 = (Frag14*)frag;
        
        if (!f14->hasMeshRefs())
            continue;
        
        const char* modelName = wld->getFragName(f14);
        if (!modelName)
            continue;
        
        Frag2d* f2d = (Frag2d*)wld->getFrag(f14->firstRef());
        if (!f2d)
            continue;
        
        uint32_t type       = f2d->type();
        ConvModel* objModel = nullptr;
        
        switch (type)
        {
        case 0x2d:
            readMesh((Frag36*)wld->getFrag(f2d->ref), true);
            objModel = new ConvModel(pfs);
            objModel->takeVertexBuffers(vbs);
            objModel->takeVertexBuffersNoCollide(cvb);
            break;
        case 0x11:
            //break;
        default:
            continue;
        }
        
        objModel->setName(modelName);
        addObjectDefinition(modelName, objModel);
    }
}

void WldModel::readObjectPlacements(WLD* wld)
{
    for (Fragment* frag : wld->getFragsByType(0x15))
    {
        Frag15* f15 = (Frag15*)frag;
        
        const char* modelName = wld->getFragName(f15->nameref);
        if (!modelName)
            continue;
        
        ConvModel* obj = getObjectDefinition(modelName);
        if (!obj)
            continue;
        
        float rotY = -(f15->rotX / 512.0f * 360.0f);
        float rotZ =   f15->rotY / 512.0f * 360.0f;
        
        Mat4 mat = Mat4::angleYZ(rotY, rotZ);
        mat.setTranslation(Vec3(f15->x, f15->z, f15->y));
        
        float scale = f15->scaleZ;
        
        if (scale != 0.0f && scale != 1.0f)
            mat = mat * Mat4::scale(scale);
        
        addObjectPlacement(mat, obj);
    }
}
