//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include "foreign_bodies_factory.hpp"


typedef immune_system::server::alien_factory af_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
    hpx::components::simple_component<af_type>
    , alien_factory_af_type);

////////////////////////////////////////////////////////////

// HPX_REGISTER_ACTION(af_type::spawn_aliens_action
//     , alien_factory_spawn_action);

HPX_REGISTER_ACTION(af_type::spawn_n_aliens_action
    , alien_factory_spawn_n_aliens_action);

HPX_REGISTER_ACTION(af_type::alien_connect_action
    , alien_factory_alien_connect_action);

HPX_REGISTER_ACTION(af_type::aliens_active_action
    , alien_factory_aliens_active_action);