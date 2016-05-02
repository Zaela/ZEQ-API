
#include "eqg_model.hpp"

EqgModel::EqgModel(PFS* pfs, byte* data, uint32_t len, bool ownsData)
: ConvModel(pfs),
  EqgCommon(data, len),
  m_ownsData(ownsData),
  m_triangles(nullptr)
{
    m_propertyHandlers["e_texturediffuse0"] = handleDiffuse;
}

EqgModel::~EqgModel()
{
    if (m_ownsData)
    {
        byte* data = getData();
        
        if (data)
            delete[] data;
    }
}

void EqgModel::loadCommon()
{
    
}

void EqgModel::loadMDS()
{
    byte* data      = getData();
    MdsHeader* h    = (MdsHeader*)data;
    uint32_t p      = sizeof(MdsHeader);
    
    checkLength(p);
    
    setVersion(h->version);
    
    p = extractStrings(p, h->stringBlockLen);
    p = extractMaterials(p, h->materialCount);
    p = extractBones(p, h->boneCount);
    
    std::vector<MdsHeadTemp> heads;
    auto& indexMap = skeleton().getIndexMap();
    
    uint32_t n = h->modelCount;

    for (uint32_t i = 0; i < n; i++)
    {
        MdsModelHeader* mh = (MdsModelHeader*)(data + p);
        
        p += sizeof(MdsModelHeader);
        checkLength(p);
        
        const char* name = getString(mh->nameIndex);
        printf("name: %s\n", name);
        
        EqgModel* model;
        
        if (name && strlen(name) >= 7 && name[3] == 'h' && name[4] == 'e' && isdigit(name[5]) && isdigit(name[6]))
        {
            model = createHeadModel();
            
            MdsHeadTemp head;
            head.headNumber = (uint32_t)((name[5] - '0') * 10 + (name[6] - '0'));
            head.model      = model;
            
            heads.push_back(head);
        }
        else
        {
            model = this;
        }
        
        p = model->extractVertexBuffers(p, mh->vertexCount, mh->triangleCount);
        p = model->extractBoneAssignments(p, mh->boneAssignmentCount, mh->triangleCount, indexMap);
    }
    
    std::sort(heads.begin(), heads.end(), [](const MdsHeadTemp& a, const MdsHeadTemp& b) { return a.headNumber < b.headNumber; });
    
    for (MdsHeadTemp& head : heads)
    {
        addHeadModel(head.model);
    }
    
    extractAnimations();
}

uint32_t EqgModel::extractMaterials(uint32_t p, uint32_t count)
{
    byte* data = getData();
    
    for (uint32_t i = 0; i < count; i++)
    {
        EqgBinMaterial* binMat = (EqgBinMaterial*)(data + p);
        p += sizeof(EqgBinMaterial);
        
        checkLength(p);
        
        const char* name = getString(binMat->nameIndex);
        
        EqgMaterial* mat = new EqgMaterial(name);
        addMaterial(mat);
        
        printf("MATERIAL: %s\n", name);
        
        for (uint32_t j = 0; j < binMat->propertyCount; j++)
        {
            EqgProperty* prop = (EqgProperty*)(data + p);
            p += sizeof(EqgProperty);
            
            checkLength(p);
            
            readProperty(mat, prop);
        }
    }
    
    return p;
}

uint32_t EqgModel::extractBones(uint32_t p, uint32_t count)
{
    EqgBone* binBones = (EqgBone*)(getData() + p);
    
    p += sizeof(EqgBone) * count;
    checkLength(p);
    
    ConvSkeleton& skele = skeleton();
    skele.init(count);
    
    for (uint32_t i = 0; i < count; i++)
    {
        EqgBone& binBone = binBones[i];
        const char* name = getString(binBone.nameIndex);
        
        skele.setBone(i, binBone.pos, binBone.rot, binBone.scale);
        skele.setAttachPointType(i, getAttachPointType(name));
        skele.addBoneNameToIndex(name, i);
    }
    
    std::function<void(uint32_t, uint32_t)> recurse;
    recurse = [&, binBones](uint32_t i, uint32_t parent)
    {
        EqgBone& binBone = binBones[i];
        
        if (binBone.linkBoneIndex != 0xffffffff)
            recurse(binBone.linkBoneIndex, parent);
        
        if (parent != 0xffffffff)
            skele.addChild(parent, i);
        
        if (binBone.childBoneIndex != 0xffffffff)
            recurse(binBone.childBoneIndex, i);
    };
    
    recurse(0, 0xffffffff);
    
    skele.buildIndexMap();
    
    return p;
}

uint32_t EqgModel::extractVertexBuffers(uint32_t p, uint32_t vertCount, uint32_t triCount, bool isZone)
{
    byte* data = getData();
    
    initVertexBuffers();
    
    EqgVertex* verts    = nullptr;
    EqgVertexV3* verts3 = nullptr;
    
    if (version() >= 3)
    {
        verts3 = (EqgVertexV3*)(data + p);
        p += vertCount * sizeof(EqgVertexV3);
    }
    else
    {
        verts = (EqgVertex*)(data + p);
        p += vertCount * sizeof(EqgVertex);
    }
    
    EqgTriangle* tris = (EqgTriangle*)(data + p);
    p += triCount * sizeof(EqgTriangle);
    
    checkLength(p);
    
    m_triangles = tris;
    
    if (isZone)
    {
        if (verts3)
        {
            for (uint32_t i = 0; i < triCount; i++)
            {
                EqgTriangle& tri    = tris[i];
                uint32_t index      = tri.materialIndex + 1;
                
                ConvVertexBuffer* vb = (tri.flag & 0x01) ? getVertexBufferNoCollide(index) : getVertexBuffer(index);
                
                if (!vb)
                    continue;
                
                for (int j = 2; j >= 0; j--)
                {
                    EqgVertexV3& s = verts3[tri.index[j]];
                    
                    Vertex v;
                    
                    v.pos       = s.pos;
                    v.normal    = s.normal;
                    v.u         = s.u;
                    v.v         = s.v;
                    
                    vb->push_back(v);
                }
            }
        }
        else
        {
            for (uint32_t i = 0; i < triCount; i++)
            {
                EqgTriangle& tri    = tris[i];
                uint32_t index      = tri.materialIndex + 1;
                
                ConvVertexBuffer* vb = (tri.flag & 0x01) ? getVertexBufferNoCollide(index) : getVertexBuffer(index);
                
                if (!vb)
                    continue;
                
                for (int j = 2; j >= 0; j--)
                {
                    EqgVertex& s = verts[tri.index[j]];
                    
                    Vertex v;
                    
                    v.pos       = s.pos;
                    v.normal    = s.normal;
                    v.u         = s.u;
                    v.v         = s.v;
                    
                    vb->push_back(v);
                }
            }
        }
    }
    else
    {
        if (verts3)
        {
            for (uint32_t i = 0; i < triCount; i++)
            {
                EqgTriangle& tri    = tris[i];
                uint32_t index      = tri.materialIndex + 1;
                
                ConvVertexBuffer* vb = (tri.flag & 0x01) ? getVertexBufferNoCollide(index) : getVertexBuffer(index);
                
                if (!vb)
                    continue;
                
                for (int j = 0; j < 3; j++)
                {
                    EqgVertexV3& s = verts3[tri.index[j]];
                    
                    Vertex v;
                    
                    v.pos       = s.pos;
                    v.normal    = s.normal;
                    v.u         = s.u;
                    v.v         = s.v;
                    
                    vb->push_back(v);
                }
            }
        }
        else
        {
            for (uint32_t i = 0; i < triCount; i++)
            {
                EqgTriangle& tri    = tris[i];
                uint32_t index      = tri.materialIndex + 1;
                
                ConvVertexBuffer* vb = (tri.flag & 0x01) ? getVertexBufferNoCollide(index) : getVertexBuffer(index);
                
                if (!vb)
                    continue;
                
                for (int j = 0; j < 3; j++)
                {
                    EqgVertex& s = verts[tri.index[j]];
                    
                    Vertex v;
                    
                    v.pos       = s.pos;
                    v.normal    = s.normal;
                    v.u         = s.u;
                    v.v         = s.v;
                    
                    vb->push_back(v);
                }
            }
        }
    }
    
    return p;
}

uint32_t EqgModel::extractBoneAssignments(uint32_t p, uint32_t count, uint32_t triCount, std::unordered_map<uint32_t, uint32_t>& indexMap)
{
    initWeightBuffers();
    
    EqgBoneAssignment* bas = (EqgBoneAssignment*)(getData() + p);
    p += count * sizeof(EqgBoneAssignment);
    
    checkLength(p);
    
    EqgTriangle* tris = m_triangles;
    
    // Need to account for triangles indexing vertices as one huge flat array while we have them separated into different buffers
    std::vector<uint32_t> vertCountsByMatIndex;
    std::vector<uint32_t> vertCountsByMatIndexNoCollide;
    
    uint32_t matCount = getAllMaterials().size();
    
    for (uint32_t i = 0; i < matCount; i++)
    {
        vertCountsByMatIndex.push_back(0);
        vertCountsByMatIndexNoCollide.push_back(0);
    }
    
    for (uint32_t i = 0; i < triCount; i++)
    {
        EqgTriangle& tri    = tris[i];
        uint32_t index      = tri.materialIndex;
        
        if (index >= matCount)
            continue;
        
        EqgWeightBuffer& wt = (tri.flag & 0x01) ? m_weightBuffersNoCollide[index] : m_weightBuffers[index];
        uint32_t& vcount    = (tri.flag & 0x01) ? vertCountsByMatIndexNoCollide[index] : vertCountsByMatIndex[index];
        
        for (uint32_t j = 0; j < 3; j++)
        {
            EqgBoneAssignment& ba   = bas[tri.index[j]];
            uint32_t vindex         = vcount + j;
            
            for (uint32_t k = 0; k < ba.count; k++)
            {
                EqgWeight& w = ba.weights[k];
                
                if (!indexMap.count(w.boneIndex))
                    continue;
                
                wt.add(vindex, indexMap[w.boneIndex], w.value);
            }
        }
        
        vcount += 3;
    }
    
    for (EqgWeightBuffer& wt : m_weightBuffers)
    {
        wt.sort();
    }
    
    for (EqgWeightBuffer& wt : m_weightBuffersNoCollide)
    {
        wt.sort();
    }
    
    return p;
}

void EqgModel::extractAnimations()
{
    PFS* pfs = getPFS();
    auto end = pfs->end();
    
    for (auto iter = pfs->begin(); iter != end; ++iter)
    {
        const std::string& name = iter.name();
        uint32_t len            = name.length();
        
        if (len < 5 || name[len - 3] != 'a' || name[len - 2] != 'n' || name[len - 1] != 'i')
            continue;
        
        printf("anim: %s\n", name.c_str());
        
        byte* data = iter.data(len);
        
        EqgAni ani(name.c_str(), data, len);
        
        ani.init();
        ani.extract(this);
    }
}

void EqgModel::initWeightBuffers()
{
    auto& vbs = getVertexBuffers();
    auto& cvb = getVertexBuffersNoCollide();
    
    for (ConvVertexBuffer& vb : vbs)
    {
        m_weightBuffers.emplace_back(vb);
    }
    
    for (ConvVertexBuffer& vb : cvb)
    {
        m_weightBuffersNoCollide.emplace_back(vb);
    }
}

EqgModel* EqgModel::createHeadModel()
{
    EqgModel* model = new EqgModel(getPFS(), getData(), length(), false);
    
    auto& materials = getAllMaterials();
    
    // Skip material 0 -- the null material, already included by the ConvModel constructor
    for (uint32_t i = 1; i < materials.size(); i++)
    {
        model->addMaterial(materials[i]);
    }

    return model;
}

AttachPoint::Type EqgModel::getAttachPointType(const std::string& boneName)
{
    return AttachPoint::Type::NONE;
}

void EqgModel::readProperty(EqgMaterial* mat, EqgProperty* prop)
{
    const char* name = getString(prop->typeNameIndex);
    
    printf("\tprop: %s\n", name);
    
    if (!name || !m_propertyHandlers.count(name))
        return;
    
    m_propertyHandlers[name](mat, prop, this);
}

void EqgModel::handleDiffuse(EqgMaterial* mat, EqgProperty* prop, EqgModel* model)
{
    if (prop->valueType != EqgProperty::Type::Index)
        return;
    
    const char* name = model->getString(prop->asIndex);
    
    if (!name)
        return;
    
    PFS* pfs        = model->getPFS();
    uint32_t len    = 0;
    byte* data      = pfs->getEntry(name, len);
    
    if (data)
        mat->addTexture(name, data, len);
}
