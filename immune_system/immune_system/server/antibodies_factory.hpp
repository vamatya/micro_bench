//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef IMMUNE_SYSTEM_ANTIBODIES_FACTORY_HPP
#define IMMUNE_SYSTEM_ANTIBODIES_FACTORY_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>

//#include <immune_system/server/foreign_bodies_factory.hpp>


namespace components
{

    // Foreign Bodies Detection and Generating Antibodies. 
    // Maintain Stat of objects created
    struct antibodies_factory
        : hpx::components::managed_component_base<antibodies_factory>
    {

        //Note: For now maintain list of all spawned antibodies. 

        //TO DO: create another header file.

        antibodies_factory(){}

        antibodies_factory(hpx::id_type my_id)
            : my_id_(my_id) 
        {}

        void init_abf();
        //{
        //}

        HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, init_abf);

        //////////////////////////////////////////////////////////////////////////
        
        void resolve_names(std::vector<hpx::id_type> ids);
        
        HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, resolve_names);

        //Sense foreign body
            // Identify each alien body, and get the no. of antibodies it has 
            // in contact. 
        // alien_factory notifies its location/id
        bool alien_factory_active();
     
        HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, alien_factory_active);

        bool get_alien_factory(hpx::id_type al_factory);
        //{
        //    al_factory_ = al_factory;
        //    return true;
        //}
        // Look into the stack of alien_factory for any aliens present. 
            // tuple of id and filled/not flag??
        
        bool scan_aliens();
        
        //Generate antibodies
            // For every contact point vaccant, generate an antibody and 
            // spawn an antibody, targetting that antibody.


        // look for identified aliens and target them. 
        // distributing factory method call components. 
        void spawn_antibody();
        //{
            //typedef ::components::alien_factory::spawn_action action_type;
        //} 
        HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, spawn_antibody);

        //Spawn \N Antibodies
        void spawn_antibodies(std::size_t num);

        HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, spawn_antibodies);

        /*std::tuple<bool, hpx::id_type> get_target(hpx::id_type antibody)
        {}*/

        //Delete Superfluous antibodies
        void kill_antibodies();
        //spawn_fb??

        //Antibodies Create Credit?
    private:
        hpx::id_type my_id_;
        hpx::id_type al_factory_;
        std::size_t produced_count;

        std::vector<hpx::id_type> fac_ids_;
        std::vector<hpx::id_type> antibodies_;
    };
}


 ////////////////////////////////////////////////////////

HPX_REGISTER_ACTION_DECLARATION(
    ::components::antibodies_factory::init_abf_action,
    antibodies_factory_init_abf_action
    );

HPX_REGISTER_ACTION_DECLARATION(
    ::components::antibodies_factory::spawn_antibody_action,
    antibodies_factory_spawn_antibody_action
    );

HPX_REGISTER_ACTION_DECLARATION(
    ::components::antibodies_factory::spawn_antibodies_action,
    antibodies_factory_spawn_antibodies_action
    );

HPX_REGISTER_ACTION_DECLARATION(
    ::components::antibodies_factory::alien_factory_active_action,
    antibodies_factory_alien_factory_active_action
    );



#endif //IMMUNE_SYSTEM_ANTIBODIES_FACTORY_HPP
