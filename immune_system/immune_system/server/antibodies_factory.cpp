
//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// #include <hpx/hpx_fwd.hpp>
// #include <hpx/hpx.hpp>
// #include <hpx/include/components.hpp>
//#include <hpx/runtime/components/server/managed_component_base.hpp>

#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/components/distributing_factory/distributing_factory.hpp>


//#include "antibodies.hpp"
#include "antibodies_factory.hpp"
//#include "foreign_bodies_factory.hpp"
//#include "foreign_bodies.hpp"



typedef immune_system::server::antibodies_factory abf_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
    hpx::components::managed_component <abf_type>
    , abf_component);

///////////////////////////////////////////////////////////////

HPX_REGISTER_ACTION(abf_type::init_abf_action
    , abf_init_abf_action);

HPX_REGISTER_ACTION(abf_type::spawn_antibody_action
    , abf_spawn_antibody_action);

HPX_REGISTER_ACTION(abf_type::spawn_antibodies_action,
    abf_spawn_antibodies_action
    );
HPX_REGISTER_ACTION(abf_type::alien_factory_active_action
    , abf_alien_factory_active_action);

//###########################################################################
