

#if !defined(IMMUNE_SYSTEM_COMMON_OBJECTS_HPP)
#define IMMUNE_SYSTEM_COMMON_OBJECTS_HPP

#include <hpx/hpx_fwd.hpp>

struct bodies
{
    hpx::id_type my_id;
    hpx::id_type foreign_object;
    bool foreign_object_attached; //fo_
public:
    bodies(){}
    bodies(bool fo_attached, hpx::id_type id)
        :foreign_object_attached(fo_attached)
        , my_id(id)
    {}
};

#endif //IMMUNE_SYSTEM_COMMON_OBJECTS_HPP
