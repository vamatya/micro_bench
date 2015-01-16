
#include <hpx/hpx.hpp>
#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>
#include <hpx/runtime/actions/component_action.hpp>

#include <hpx/components/distributing_factory/distributing_factory.hpp>

#include <immune_system/immune_system/server/antibodies.hpp>
#include <immune_system/immune_system/server/antibodies_factory.hpp>


HPX_REGISTER_COMPONENT_MODULE();

/*HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
hpx::components::managed_component< ::components::alien_factory>
, alien_factory_component);

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
hpx::components::managed_component< ::components::aliens>
, aliens_component);*/

// HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
// hpx::components::managed_component< ::components::antibodies_factory>
// , antibodies_factory_component);

typedef ::components::server::antibodies antibodies_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
    hpx::components::managed_component< antibodies_type>
    , antibodies_component);
     
//////////////////////////////////////////////////////////

HPX_REGISTER_ACTION(antibodies_type::alien_connect_action
    , antibodies_alien_connect_action);

    // 
    // HPX_REGISTER_ACTION(ab_factory_type::init_abf_action
    //     , antibodies_factory_init_abf_action);
    // 
    // HPX_REGISTER_ACTION(ab_factory_type::spawn_antibody_action
    //     , antibodies_factory_spawn_antibody_action);
    // 
    // HPX_REGISTER_ACTION(
    //     ::components::antibodies_factory::spawn_antibodies_action,
    //     antibodies_factory_spawn_antibodies_action
    //     );
    // 
    // HPX_REGISTER_ACTION(ab_factory_type::alien_factory_active_action
    //     , antibodies_factory_alien_factory_active_action);