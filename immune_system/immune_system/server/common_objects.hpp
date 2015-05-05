

#if !defined(IMMUNE_SYSTEM_COMMON_OBJECTS_HPP)
#define IMMUNE_SYSTEM_COMMON_OBJECTS_HPP

#include <hpx/hpx_fwd.hpp>

struct bodies
{
    hpx::id_type my_id;
    hpx::id_type foreign_object;
    bool foreign_object_attached; 
    bool migrated;

    friend class hpx::serialization::access;
    template <class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & my_id;
        ar & foreign_object;
        ar & foreign_object_attached;
        ar & migrated;
    }

public:
    bodies()
        :foreign_object_attached(false)
        , migrated(false)
    {
    }

    bodies(hpx::id_type id)
        :my_id(id)
    {}

    bodies(bool fo_attached, hpx::id_type id)
        :foreign_object_attached(fo_attached)
        , my_id(id)
    {}
};

#endif //IMMUNE_SYSTEM_COMMON_OBJECTS_HPP
