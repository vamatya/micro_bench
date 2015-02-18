//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include "foreign_bodies.hpp"


typedef immune_system::server::aliens aliens_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
    hpx::components::simple_component < aliens_type >
    , aliens_aliens_type);


HPX_REGISTER_ACTION(aliens_type::ab_connect_action
    , aliens_ab_connect_action);

HPX_REGISTER_ACTION(aliens_type::alien_active_action
    , aliens_alien_active_action);

HPX_REGISTER_ACTION(aliens_type::send_spawn_signal_action
    , aliens_send_spawn_signal_action);
