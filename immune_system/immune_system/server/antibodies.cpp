//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// #include <hpx/hpx_fwd.hpp>
// #include <hpx/include/components.hpp>
// //#include <hpx/runtime/components/server/managed_component_base.hpp>
// #include <hpx/runtime/actions/component_action.hpp>

//#include "function_registry.hpp"
#include "antibodies.hpp"



typedef ::components::server::antibodies antibodies_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
    hpx::components::managed_component < antibodies_type >
    , antibodies_component);

//////////////////////////////////////////////////////////

HPX_REGISTER_ACTION(antibodies_type::alien_connect_action
    , antibodies_alien_connect_action);

