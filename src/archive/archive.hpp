
#ifndef _ZEQ_ARCHIVE_HPP_
#define _ZEQ_ARCHIVE_HPP_

#include "define.hpp"
#include "zeq_object.hpp"
#include "path.hpp"
#include "pfs.hpp"
#include "wld.hpp"
#include "file.hpp"
#include "model_listing.hpp"
#include "wld_model.hpp"
#include "eqg_model.hpp"
#include "zone_model.hpp"
#include <vector>
#include <unordered_map>

struct zeq_archive_t : public ZeqObject
{
private:
    const std::string m_fileName;

    bool m_isEQG;
    bool m_isZone;

    PFS* m_mainPfs;

    std::vector<ModelListing> m_modelList;

    union
    {
        struct
        {
            WLD* m_mainWld;
            WLD* m_objLocWld;
            PFS* m_objDefPfs;
            WLD* m_objDefWld;
            PFS* m_2Pfs;
            WLD* m_2Wld;
        };
        struct
        {
            void* m_zon;
            void* m_ter;
        };
    };
    
private:
    zeq_zone_model_t* loadZoneWLD(uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects);
    zeq_zone_model_t* loadZoneEQG(uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects);

    zeq_model_proto_t*  loadModelWLD(const std::string& name, zeq_model_proto_t* inheritAnimationsFrom);
    zeq_model_proto_t*  loadModelEQG(const std::string& name);
    
public:
    zeq_archive_t(zeq_t* Z, const zeq_path_t& path, const std::string& filename);
    ~zeq_archive_t();

    bool isZone() const { return m_isZone; }

    const std::string& getFileName() { return m_fileName; }
    
    const std::vector<ModelListing>& getModelList() const { return m_modelList; }
    
    zeq_zone_model_t*   loadZone(uint32_t maxTrianglesPerNode = 512, int useStaticGeometryForObjects = 1);
    zeq_model_proto_t*  loadModel(const std::string& name, zeq_model_proto_t* inheritAnimationsFrom);
};

#endif//_ZEQ_ARCHIVE_HPP_
