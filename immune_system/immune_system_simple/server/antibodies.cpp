//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/actions/continuation.hpp>
#include <hpx/runtime/components/component_factory.hpp>
#include <hpx/runtime.hpp>

#include <hpx/util/portable_binary_iarchive.hpp>
#include <hpx/util/portable_binary_oarchive.hpp>

#include "antibodies.hpp"



typedef immune_system::server::antibodies antibodies_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
    hpx::components::managed_component < antibodies_type >
    , antibodies_component);

//////////////////////////////////////////////////////////

HPX_REGISTER_ACTION(antibodies_type::alien_connect_action
    , antibodies_alien_connect_action);

