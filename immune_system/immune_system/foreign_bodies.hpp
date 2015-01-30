//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/components.hpp>
#include <hpx/components/distributing_factory/distributing_factory.hpp>

#include<immune_system/immune_system/server/foreign_bodies_factory.hpp>
//#include <immune_system/immune_system/server/foreign_bodies.hpp>


struct alien_factory_client
    : hpx::components::client_base<alien_factory_client, immune_system::server::alien_factory>
{
    typedef hpx::components::client_base < alien_factory_client
        , immune_system::server::alien_factory > base_type;

    alien_factory_client(){}
    
};
