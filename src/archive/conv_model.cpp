
#include "conv_model.hpp"

ConvModel::ConvModel(PFS* pfs)
: m_pfs(pfs)
{
    // First material is always the Null material
    m_materials.push_back(ConvMaterial::Null());
}

ConvModel::~ConvModel()
{
    ConvMaterial* nullMat = ConvMaterial::Null();
    
    for (ConvMaterial* mat : m_materials)
    {
        if (mat && mat != nullMat)
            delete mat;
    }
    
    for (ConvModel* head : m_headModels)
    {
        if (head)
            delete head;
    }
    
    for (auto& p : m_objectModelsByName)
    {
        if (p.second)
            delete p.second;
    }
}

void ConvModel::initVertexBuffers()
{
    uint32_t count = m_materials.size();
    
    m_vertexBuffers.resize(count);
    m_noCollideVertexBuffers.resize(count);
    
    for (uint32_t i = 0; i < count; i++)
    {
        ConvMaterial* mat = m_materials[i];
        
        m_vertexBuffers[i].setMaterial(mat);
        m_noCollideVertexBuffers[i].setMaterial(mat);
    }
}

ConvVertexBuffer* ConvModel::getVertexBuffer(uint32_t index)
{
    if (index >= m_vertexBuffers.size())
        return nullptr;
    return &m_vertexBuffers[index];
}

ConvVertexBuffer* ConvModel::getVertexBufferNoCollide(uint32_t index)
{
    if (index >= m_noCollideVertexBuffers.size())
        return nullptr;
    return &m_noCollideVertexBuffers[index];
}

void ConvModel::prepareTextures(zeq_t* Z)
{
    for (ConvMaterial* mat : m_materials)
    {
        mat->prepareTextures(Z);
    }
}

void ConvModel::takeVertexBuffers(std::vector<ConvVertexBuffer>& src, std::vector<ConvVertexBuffer>& dst)
{
    for (ConvVertexBuffer& vb : src)
    {
        if (!vb.empty())
            dst.push_back(std::move(vb));
    }
}

void ConvModel::takeVertexBuffers(std::vector<ConvVertexBuffer>& src)
{
    takeVertexBuffers(src, m_vertexBuffers);
}

void ConvModel::takeVertexBuffersNoCollide(std::vector<ConvVertexBuffer>& src)
{
    takeVertexBuffers(src, m_noCollideVertexBuffers);
}

bool ConvModel::isAnimated() const
{
    return false; //fixme
}

ConvModel* ConvModel::getObjectDefinition(const char* name)
{
    if (m_objectModelsByName.count(name) == 0)
        return nullptr;
    return m_objectModelsByName[name];
}

void ConvModel::addObjectDefinition(const char* name, ConvModel* obj)
{
    if (m_objectModelsByName.count(name) != 0)
        return;
    
    m_objectModelsByName[name] = obj;
}

void ConvModel::addObjectPlacement(Mat4& transform, ConvModel* obj)
{
    ObjectPlacement p;
    
    p.model     = obj;
    p.transform = transform;
    
    if (obj->isAnimated())
        m_objectPlacementsAnimated.push_back(p);
    else
        m_objectPlacementsStatic.push_back(p);
}

void ConvModel::addObjectsAsStaticGeometry(ConvModel& objModels)
{
    std::unordered_map<ConvMaterial*, uint32_t> indicesByMaterial;
    
    auto take = [&](Mat4& matrix, std::vector<ConvVertexBuffer>& src, std::vector<ConvVertexBuffer>& dst)
    {
        for (ConvVertexBuffer& vb : src)
        {
            ConvMaterial* mat = vb.getMaterial();
            
            uint32_t index;
            
            if (indicesByMaterial.count(mat))
            {
                index = indicesByMaterial[mat];
            }
            else
            {
                index = dst.size();
                dst.emplace_back(mat);
                dst[index].reserve(src.size());
                indicesByMaterial[mat] = index;
            }
            
            ConvVertexBuffer& d = dst[index];
            
            for (Vertex& vert : vb)
            {
                Vertex v = vert;
                
                matrix.transformVector(v.pos);
                
                d.push_back(v);
            }
        }
    };
    
    for (ObjectPlacement& p : objModels.m_objectPlacementsStatic)
    {
        ConvModel* obj = p.model;
        
        take(p.transform, obj->getVertexBuffers(), getVertexBuffers());
        take(p.transform, obj->getVertexBuffersNoCollide(), getVertexBuffersNoCollide());
    }
}
