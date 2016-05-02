
#ifndef _ZEQ_H_
#define _ZEQ_H_

#include <stdint.h>

#if defined(_WIN32) || defined(WIN32)
 #define ZEQ_WINDOWS
#endif

#ifdef __cplusplus
 #ifdef ZEQ_WINDOWS
  #define ZEQ_API extern "C" __declspec(dllexport)
 #else
  #define ZEQ_API extern "C"
 #endif
#else
 #ifdef ZEQ_WINDOWS
  #define ZEQ_API __declspec(dllexport)
 #else
  #define ZEQ_API extern
 #endif
#endif

/*
** Types
*/

/* Common */
typedef struct zeq_t zeq_t;

/* Timer */
typedef struct zeq_timer_t  zeq_timer_t;
typedef void(*zeq_timer_callback_t)(zeq_timer_t*);

/* Archive */
typedef struct zeq_archive_t    zeq_archive_t;
typedef struct zeq_model_name_t zeq_model_name_t;

enum zeq_model_type_t
{
    ZEQ_MODEL_ZONE,
    ZEQ_MODEL_OBJECT,
    ZEQ_MODEL_MOB,
    ZEQ_MODEL_ITEM
};

struct zeq_model_name_t
{
    const char*         name;
    zeq_model_type_t    type;
};

/* Graphics */
typedef struct zeq_texture_t        zeq_texture_t;
typedef struct zeq_anim_texture_t   zeq_anim_texture_t;
typedef struct zeq_texture_set_t    zeq_texture_set_t;
typedef struct zeq_material_t       zeq_material_t;
typedef struct zeq_zone_model_t     zeq_zone_model_t;
typedef struct zeq_model_proto_t    zeq_model_proto_t;
typedef struct zeq_model_inst_t     zeq_model_inst_t;
typedef struct zeq_camera_t         zeq_camera_t;
typedef struct zeq_delta_t          zeq_delta_t;

typedef struct zeq_color_t          zeq_color_t;
typedef struct zeq_vec3_t           zeq_vec3_t;

struct zeq_color_t
{
    uint8_t r, g, b, a;
};

enum zeq_blend_t
{
    ZEQ_BLEND_SOLID,
    ZEQ_BLEND_MASKED,
    ZEQ_BLEND_PARTICLE,
    ZEQ_BLEND_ADDITIVE,
    ZEQ_BLEND_ALPHA,
    ZEQ_BLEND_INVISIBLE,
    ZEQ_BLEND_COUNT
};

struct zeq_delta_t
{
    double      seconds;
    uint64_t    microseconds;
};

struct zeq_vec3_t
{
    float x, y, z;
};

/*
** Functions
*/

/* Common */
ZEQ_API zeq_t*              zeq_init(void);
ZEQ_API void                zeq_deinit(zeq_t* Z);
ZEQ_API void                zeq_opengl_context_created(zeq_t* Z);
ZEQ_API void                zeq_thread_sleep(uint32_t milliseconds);
ZEQ_API uint64_t            zeq_clock_get_microseconds(void);
ZEQ_API void                zeq_set_global_background_color(float r, float g, float b, float a);

ZEQ_API void                zeq_set_path_to_eq(zeq_t* Z, const char* path);
ZEQ_API const char*         zeq_get_path_to_eq(zeq_t* Z);

/* Generic Getters and Setters */
enum zeq_type_t
{
    ZEQ_TIMER,
    ZEQ_CAMERA
};

ZEQ_API int                 zeq_get_int(zeq_type_t type, void* object, int field);
ZEQ_API uint32_t            zeq_get_uint(zeq_type_t type, void* object, int field);
ZEQ_API float               zeq_get_float(zeq_type_t type, void* object, int field);
ZEQ_API zeq_vec3_t          zeq_get_vec3(zeq_type_t type, void* object, int field);
ZEQ_API void*               zeq_get_ptr(zeq_type_t type, void* object, int field);

ZEQ_API void                zeq_set_int(zeq_type_t type, void* object, int field, int value);
ZEQ_API void                zeq_set_uint(zeq_type_t type, void* object, int field, uint32_t value);
ZEQ_API void                zeq_set_float(zeq_type_t type, void* object, int field, float value);
ZEQ_API void                zeq_set_vec3(zeq_type_t type, void* object, int field, float x, float y, float z);
ZEQ_API void                zeq_set_ptr(zeq_type_t type, void* object, int field, void* value);

/* Shortcut model loaders */
/*ZEQ_API zeq_zone_model_t*   zeq_load_zone(zeq_t* Z, const char* shortname);*/
/*ZEQ_API zeq_zone_model_t*   zeq_load_zone_ex(zeq_t* Z, const char* shortname, uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects);*/
/*ZEQ_API zeq_model_proto_t*  zeq_load_mob(zeq_t* Z, uint16_t raceId, uint8_t genderId);*/
/*ZEQ_API zeq_model_proto_t*  zeq_load_item(zeq_t* Z, uint32_t itemId);*/

/* Timer */
ZEQ_API zeq_timer_t*        zeq_timer_start(zeq_t* Z, uint32_t periodMilliseconds, zeq_timer_callback_t callback, void* userdata);
ZEQ_API void                zeq_timer_stop(zeq_timer_t* timer);
ZEQ_API void                zeq_timer_restart(zeq_timer_t* timer);
ZEQ_API void                zeq_timer_destroy(zeq_timer_t* timer);

enum zeq_timer_field_t
{
    ZEQ_TIMER_USERDATA_PTR,
    ZEQ_TIMER_PERIOD_UINT
};

/* Archive */
ZEQ_API zeq_archive_t*      zeq_archive_open(zeq_t* Z, const char* path);
ZEQ_API void                zeq_archive_close(zeq_archive_t* archive);

ZEQ_API int                 zeq_archive_get_model_list(zeq_archive_t* archive, zeq_model_name_t* array, int count);
ZEQ_API int                 zeq_archive_is_zone(zeq_archive_t* archive);
ZEQ_API zeq_zone_model_t*   zeq_archive_load_zone(zeq_archive_t* archive);
ZEQ_API zeq_zone_model_t*   zeq_archive_load_zone_ex(zeq_archive_t* archive, uint32_t maxTrianglesPerNode, int useStaticGeometryForObjects);
ZEQ_API zeq_model_proto_t*  zeq_archive_load_model(zeq_archive_t* archive, const char* name, zeq_model_proto_t* inheritAnimationsFrom);

/* Draw Loop */
ZEQ_API zeq_delta_t         zeq_advance(zeq_t* Z);
ZEQ_API void                zeq_draw_begin(void);
ZEQ_API void                zeq_draw_end(void);
/*ZEQ_API void                zeq_animate_zone_objects(zeq_zone_model_t* zone, zeq_delta_t delta);
ZEQ_API void                zeq_animate_all_mobs(zeq_t* Z, zeq_delta_t delta);
ZEQ_API void                zeq_draw_all_mobs(zeq_t* Z, zeq_camera_t* camera);

ZEQ_API zeq_delta_t         zeq_default_loop_step(zeq_t* Z, zeq_zone_model_t* zone, zeq_camera_t* camera);*/

/* Camera */
ZEQ_API zeq_camera_t*       zeq_camera_create(void);
ZEQ_API void                zeq_camera_destroy(zeq_camera_t* camera);

ZEQ_API void                zeq_camera_set_fog(zeq_camera_t* camera, float r, float g, float b, float density, int useExp2Equation);

enum zeq_camera_field_t
{
    ZEQ_CAMERA_POSITION_VEC3,
    ZEQ_CAMERA_LOOK_AT_VEC3,
    ZEQ_CAMERA_UP_VECTOR_VEC3,
    ZEQ_CAMERA_FIELD_OF_VIEW_DEGREES_FLOAT,
    ZEQ_CAMERA_FIELD_OF_VIEW_RADIANS_FLOAT,
    ZEQ_CAMERA_ASPECT_RATIO_FLOAT,
    ZEQ_CAMERA_FAR_CLIP_DISTANCE_FLOAT,
    ZEQ_CAMERA_NEAR_CLIP_DISTANCE_FLOAT
};

/* Zone */
ZEQ_API void                zeq_zone_destroy(zeq_zone_model_t* zone);
ZEQ_API void                zeq_zone_draw(zeq_zone_model_t* zone, zeq_camera_t* camera);

/* Model Prototype */
ZEQ_API void                zeq_prototype_destroy(zeq_model_proto_t* proto);

/* Model Instance */
ZEQ_API zeq_model_inst_t*   zeq_model_create(zeq_model_proto_t* proto);
ZEQ_API void                zeq_model_destroy(zeq_model_inst_t* inst);

ZEQ_API void                zeq_model_draw(zeq_model_inst_t* inst, zeq_camera_t* camera);

#endif/*_ZEQ_H_*/
