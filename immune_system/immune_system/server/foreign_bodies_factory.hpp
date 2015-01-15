//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef IMMUNE_SYSTEM_FOREIGN_BODIES_FACTORY_HPP
#define IMMUNE_SYSTEM_FOREIGN_BODIES_FACTORY_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
//#include <hpx/runtime/components/server/managed_component_base.hpp>
#include <hpx/runtime/actions/component_action.hpp>


#include <immune_system/immune_system/server/foreign_bodies.hpp>
#include <immune_system/immune_system/server/antibodies_factory.hpp>


namespace components
{
    // Foreign(Alien) Body Generating Factory. 

    struct alien_factory
        : hpx::components::managed_component_base<alien_factory>
    {
        //Note: For now maintain list of all spawned aliens. 

        // Generate Aliens
            // Based on the feedback from initial alien created, generate 
            // more aliens. 

        void init();
        //{}
        HPX_DEFINE_COMPONENT_ACTION(alien_factory, init);


        void spawn();
        //{
            /*
            hpx::components::component_type c_type = 
                hpx::components::get_component_type<components::aliens>();

            hpx::id_type this_locality = hpx::find_here(); //??

            using hpx::components::distributing_factory;

            typedef 
                hpx::components::server::runtime_support::bulk_create_components_action 
                action_type;

            typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;

            if(this_locality)
            {
                future_type f;
                {
                    hpx::lcos::packaged_action<action_type
                        , std::vector<hpx::naming::gid_type> > p;
                    p.apply(hpx::launch::async, this_locality, c_type, 1);
                    f = p.get_future(); 
                }
                ////converting gid_type to id_type??
                //aliens_.push_back(f.get());
            }

            */
            //typedef ::components::aliens::ab_connect_action act_type;

        //}
        HPX_DEFINE_COMPONENT_ACTION(alien_factory, spawn);

        void spawn_n_aliens(std::size_t num);
        //{
        //}

        HPX_DEFINE_COMPONENT_ACTION(alien_factory, spawn_n_aliens);

        // Aliens Create Credit?
    private:
        std::vector<hpx::id_type> aliens_;
    };

}
//////////////////////////////////////////////////////////////////

HPX_REGISTER_ACTION_DECLARATION(
    ::components::alien_factory::spawn_action,
    alien_factory_spawn_action
    );

HPX_REGISTER_ACTION_DECLARATION(
    ::components::alien_factory::spawn_n_aliens_action,
    alien_factory_spawn_n_aliens_action
    );

#endif //IMMUNE_SYSTEM_FOREIGN_BODIES_FACTORY_HPP