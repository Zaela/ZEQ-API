
#include "archive.hpp"
#include "zeq_struct.hpp"

zeq_archive_t::zeq_archive_t(zeq_t* Z, const zeq_path_t& path, const std::string& filename)
: ZeqObject(Z),
  m_fileName(filename),
  m_isZone(false),
  m_mainWld(nullptr),
  m_objLocWld(nullptr),
  m_objDefPfs(nullptr),
  m_objDefWld(nullptr),
  m_2Pfs(nullptr),
  m_2Wld(nullptr)
{
    m_mainPfs   = new PFS(path.str());
    m_isEQG     = !m_mainPfs->hasWLD();
    
    if (m_isEQG)
    {
        auto end = m_mainPfs->end();
        for (auto iter = m_mainPfs->begin(); iter != end; ++iter)
        {
            const std::string& name = iter.name();
            uint32_t len            = name.length();
            
            if (len > 4 && name[len - 3] == 'm' && ((name[len - 2] == 'o' && name[len - 1] == 'd') || (name[len - 2] == 'd' && name[len - 1] == 's')))
            {
                m_modelList.push_back(ModelListing(name.c_str(), (name[0] == 'i' && name[1] == 't') ? ZEQ_MODEL_ITEM : ZEQ_MODEL_MOB));
            }
        }
    }
    else
    {
        uint32_t len    = 0;
        byte* data      = nullptr;
        std::string str;
        
        bool isGEquip = (filename.find("gequip") != std::string::npos);
        
        // Main WLD
        str     = filename + ".wld";
        data    = m_mainPfs->getEntry(str, len);
        
        if (data && len)
        {
            m_mainWld   = new WLD(data, len);
            m_isZone    = m_mainWld->isZone();
            
            if (m_isZone)
                m_modelList.push_back(ModelListing(m_fileName.c_str(), ZEQ_MODEL_ZONE));
            else
                m_mainWld->listModels(m_modelList, isGEquip ? ZEQ_MODEL_ITEM : ZEQ_MODEL_MOB);
        }
        
        // Object Placements WLD
        str     = "objects.wld";
        data    = m_mainPfs->getEntry(str, len);
        
        if (data && len)
            m_objLocWld = new WLD(data, len);
        
        // Object Definition PFS and WLD
        str = path.insertBeforeExtension("_obj");
        
        if (File::exists(str))
        {
            m_objDefPfs = new PFS(str);
            
            str     = filename + "_obj.wld";
            data    = m_objDefPfs->getEntry(str, len);
            
            if (data && len)
            {
                m_objDefWld = new WLD(data, len);
                m_objDefWld->listModels(m_modelList, ZEQ_MODEL_OBJECT);
            }
        }
        
        // Extra data PFS and WLD (luclin models, mainly)
        // Don't count gequip or global_chr since they have several versions each
        if (!isGEquip && filename.find("global_chr") == std::string::npos)
        {
            str = path.insertBeforeExtension("2");
            
            if (File::exists(str))
            {
                m_2Pfs = new PFS(str);
                
                str     = filename + "2.wld";
                data    = m_2Pfs->getEntry(str, len);
                
                if (data && len)
                    m_2Wld = new WLD(data, len);
            }
        }
    }
}

zeq_archive_t::~zeq_archive_t()
{
    if (m_mainPfs)
        delete m_mainPfs;
    
    if (m_isEQG)
    {
        
    }
    else
    {
        if (m_mainWld)
            delete m_mainWld;
        
        if (m_objLocWld)
            delete m_objLocWld;
        
        if (m_objDefPfs)
            delete m_objDefPfs;
        
        if (m_objDefWld)
            delete m_objDefWld;
        
        if (m_2Pfs)
            delete m_2Pfs;
        
        if (m_2Wld)
            delete m_2Wld;
    }
}

zeq_zone_model_t* zeq_archive_t::loadZone(uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects)
{
    if (!isZone())
        return nullptr;
    
    zeq_zone_model_t* zone;
    
    if (!m_isEQG)
        zone = loadZoneWLD(maxTrianglesPerNode, useStaticGeometryForObjects);
    else
        zone = loadZoneEQG(maxTrianglesPerNode, useStaticGeometryForObjects);
    
    return zone;
}

zeq_zone_model_t* zeq_archive_t::loadZoneWLD(uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects)
{
    WldModel zoneModel(m_mainPfs, m_mainWld);
    
    zoneModel.readAllMaterials();
    zoneModel.readAllMeshes();
    
    WldModel defModel(m_objDefPfs, m_objDefWld);
    
    defModel.readAllMaterials();
    defModel.readObjectDefinitions();
    defModel.readObjectPlacements(m_objLocWld);

    return new zeq_zone_model_t(Z(), m_fileName, zoneModel, defModel, maxTrianglesPerNode, useStaticGeometryForObjects);
}

zeq_zone_model_t* zeq_archive_t::loadZoneEQG(uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects)
{
    return nullptr;
}

zeq_model_proto_t* zeq_archive_t::loadModel(const std::string& name, zeq_model_proto_t* inheritAnimationsFrom)
{
    zeq_model_proto_t* proto;
    
    if (!m_isEQG)
        proto = loadModelWLD(name, inheritAnimationsFrom);
    else
        proto = loadModelEQG(name);
    
    return proto;
}

zeq_model_proto_t* zeq_archive_t::loadModelWLD(const std::string& name, zeq_model_proto_t* inheritAnimationsFrom)
{
    WLD* wld = m_mainWld;
    
    if (!wld)
        return nullptr;
    
    WldModel model(m_mainPfs, wld);
    model.readModel(name.c_str());
    
    bool isItem = (name[0] == 'i' && name[1] == 't');
    
    zeq_model_proto_t* proto = new zeq_model_proto_t(model, isItem ? ZEQ_MODEL_ITEM : ZEQ_MODEL_MOB, inheritAnimationsFrom, zeq_model_proto_t::BaseTransform::Wld);
        
    proto->grab();
    return proto;
}

zeq_model_proto_t* zeq_archive_t::loadModelEQG(const std::string& name)
{
    uint32_t length = name.length();
    
    if (length < 4)
        return nullptr;
    
    auto end = m_mainPfs->end();
    
    for (auto iter = m_mainPfs->begin(); iter != end; ++iter)
    {
        if (name.compare(iter.name()) != 0)
            continue;
        
        uint32_t len    = 0;
        byte* data      = iter.data(len);
        
        if (!data)
            continue;
        
        EqgModel model(m_mainPfs, data, len);
        
        if (name[length - 3] == 'm' && name[length - 2] == 'd' && name[length - 1] == 's')
            model.loadMDS();
        else
            model.loadCommon();
        
        zeq_model_proto_t* proto = new zeq_model_proto_t(model, ZEQ_MODEL_MOB, nullptr, zeq_model_proto_t::BaseTransform::Eqg);
        
        proto->grab();
        return proto;
    }
    
    return nullptr;
}

/*
** API Functions
*/

zeq_archive_t* zeq_archive_open(zeq_t* Z, const char* path)
{
    zeq_path_t p = Z->getPathToEQ();
    p += path;
    
    if (!File::exists(p.str()))
    {
        zeq_path_t temp = p;
        temp += ".eqg";
        
        if (File::exists(temp.str()))
        {
            p = temp;
        }
        else
        {
            p += ".s3d";
            if (!File::exists(p.str()))
                return nullptr;
        }
    }
    
    try
    {
        std::string fileName    = p.getFileNameNoExtension();
        zeq_archive_t* arch     = new zeq_archive_t(Z, p, fileName);

        return arch;
    }
    catch (...)
    {
        return nullptr;
    }
}

void zeq_archive_close(zeq_archive_t* arch)
{
    if (!arch)
        return;

    delete arch;
}

int zeq_archive_get_model_list(zeq_archive_t* arch, zeq_model_name_t* array, int count)
{
    const auto& list    = arch->getModelList();
    int size            = (int)list.size();
    
    if (!array || !count)
        goto ret;
    
    if (count > size)
        count = size;
    
    for (int i = 0; i < count; i++)
    {
        const auto& m       = list[i];
        zeq_model_name_t& n = array[i];
        
        n.name = m.name;
        n.type = (zeq_model_type_t)m.type;
    }
    
ret:
    return size;
}

int zeq_archive_is_zone(zeq_archive_t* arch)
{
    return (int)arch->isZone();
}

zeq_zone_model_t* zeq_archive_load_zone(zeq_archive_t* arch)
{
    try
    {
        return arch->loadZone();
    }
    catch (...)
    {
        return nullptr;
    }
}

zeq_zone_model_t* zeq_archive_load_zone_ex(zeq_archive_t* arch, uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects)
{
    try
    {
        return arch->loadZone(maxTrianglesPerNode, useStaticGeometryForObjects);
    }
    catch (...)
    {
        return nullptr;
    }
}

zeq_model_proto_t* zeq_archive_load_model(zeq_archive_t* arch, const char* name, zeq_model_proto_t* inheritAnimationsFrom)
{
    try
    {
        return arch->loadModel(name, inheritAnimationsFrom);
    }
    catch (...)
    {
        return nullptr;
    }
}
