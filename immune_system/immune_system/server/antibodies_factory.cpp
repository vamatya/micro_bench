
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

#include "antibodies_factory.hpp"




typedef immune_system::server::antibodies_factory abf_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
    hpx::components::managed_component <abf_type>
    , abf_component);

///////////////////////////////////////////////////////////////

HPX_REGISTER_ACTION(abf_type::init_abf_action
    , abf_init_abf_action);

//HPX_REGISTER_ACTION(abf_type::spawn_antibody_action
//    , abf_spawn_antibody_action);

HPX_REGISTER_ACTION(abf_type::spawn_antibodies_action,
    abf_spawn_antibodies_action
    );
HPX_REGISTER_ACTION(abf_type::alien_factory_active_action
    , abf_alien_factory_active_action);
HPX_REGISTER_ACTION(abf_type::print_stat_action
    , abf_print_stat_action);
HPX_REGISTER_ACTION(abf_type::kill_antibodies_action
    , abf_kill_antibodies_action);

//###########################################################################
