
#include <hpx/hpx.hpp>
#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>
#include <hpx/runtime/actions/component_action.hpp>

#include <hpx/components/distributing_factory/distributing_factory.hpp>

#include <immune_system/immune_system/server/antibodies.hpp>
#include <immune_system/immune_system/server/antibodies_factory.hpp>


//HPX_REGISTER_COMPONENT_MODULE();

// typedef ::components::server::antibodies_factory ab_factory_type;
    // 
    // HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
    // hpx::components::managed_component< ab_factory_type>
    // , antibodies_factory_component);
    // 
    // ///////////////////////////////////////////////////////////////
    // 
    // HPX_REGISTER_ACTION(ab_factory_type::init_abf_action
    //     , antibodies_factory_init_abf_action);
    // 
    // HPX_REGISTER_ACTION(ab_factory_type::spawn_antibody_action
    //     , antibodies_factory_spawn_antibody_action);
    // 
    // HPX_REGISTER_ACTION(ab_factory_type::spawn_antibodies_action,
    //     antibodies_factory_spawn_antibodies_action
    //     );
    // HPX_REGISTER_ACTION(ab_factory_type::alien_factory_active_action
    //     , antibodies_factory_alien_factory_active_action);