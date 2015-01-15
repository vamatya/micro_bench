//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


//#include "function_registry.hpp"
#include "antibodies.hpp"


bool components::antibodies::alien_connect(hpx::id_type target)
{
    // update target. 
    target_ = target;
    //typedef components::aliens::ab_connect_action action_type;
    //hpx::future<bool> connect = hpx::async<action_type>(target);
    //TEST CODE, remove later
    //typedef components::alien_factory::spawn_action action_type2;
    //typedef components::antibodies_factory::spawn_antibodies_action action_type3;

    //typedef ::components::alien_factory::spawn_action act_type;

    //////////////////////////////////////////////////////////////

    return true;
}

bool components::antibodies::send_spawn_signal()
{
    //typedef ::components::antibodies_factory::spawn_antibodies_action
    //    act_type;
    //typedef ::components::alien_factory::spawn_action act_type2;
    return true;
}



/////////////////////////////////////////////////////////////////////////////

//typedef ::components::antibodies antibodies_type;
typedef ::components::antibodies antibodies_type;

//HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
//    hpx::components::managed_component<antibodies_type>, ab_antibodies_type);

HPX_REGISTER_ACTION(antibodies_type::alien_connect_action
    , antibodies_alien_connect_action);

/////////////////////////////////////////////////////////////////////////////