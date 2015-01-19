//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef IMMUNE_SYSTEM_SERVER_ANTIBODIES_FACTORY_HPP
#define IMMUNE_SYSTEM_SERVER_ANTIBODIES_FACTORY_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>

//#include <immune_system/server/foreign_bodies_factory.hpp>


namespace immune_system{
    namespace server
    {
        // Foreign Bodies Detection and Generating Antibodies. 
        // Maintain Stat of objects created
        struct antibodies_factory
            : hpx::components::managed_component_base < antibodies_factory >
        {

            //Note: For now maintain list of all spawned antibodies. 

            //TO DO: create another header file.

            antibodies_factory();// {}

            antibodies_factory(hpx::id_type my_id)
                : my_id_(my_id)
            {}

            void init_abf()
            {
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, init_abf);

            //////////////////////////////////////////////////////////////////////////

            void resolve_names(std::vector<hpx::id_type> ids)
            {
                fac_ids_ = ids;
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, resolve_names);

            //Sense foreign body
            // Identify each alien body, and get the no. of antibodies it has 
            // in contact. 
            // alien_factory notifies its location/id
            bool alien_factory_active()
            {
                return false;
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, alien_factory_active);

            bool get_alien_factory(hpx::id_type al_factory)
            {
                al_factory_ = al_factory;
                return true;
            }
            //{
            //    al_factory_ = al_factory;
            //    return true;
            //}
            // Look into the stack of alien_factory for any aliens present. 
            // tuple of id and filled/not flag??

            bool scan_aliens()
            {
                return false;
            }

            //Generate antibodies
            // For every contact point vaccant, generate an antibody and 
            // spawn an antibody, targetting that antibody.


            // look for identified aliens and target them. 
            // distributing factory method call components. 
            void spawn_antibody()
            {

            }
            //{
            //typedef ::components::alien_factory::spawn_action action_type;
            //} 
            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, spawn_antibody);

            //Spawn \N Antibodies
            void spawn_antibodies(std::size_t num)
            {
                typedef hpx::util::remote_locality_result value_type;
                typedef std::pair<std::size_t, std::vector<value_type> > result_type;

                result_type res;


                typedef std::vector<hpx::id_type> id_vector_type;

                hpx::components::component_type c_type =
                    hpx::components::get_component_type<::components::server::antibodies>();

                hpx::id_type this_loc = hpx::find_here();

                typedef
                    hpx::components::server::runtime_support::bulk_create_components_action
                    action_type;

                typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;

                future_type f;
                {
                    hpx::lcos::packaged_action < action_type
                        , std::vector<hpx::naming::gid_type> > p;
                    p.apply(hpx::launch::async, this_loc, c_type, num);
                    f = p.get_future();
                }


                res.first = num;
                res.second.back().gids_ = boost::move(f.get());

                antibodies_.reserve(num);

                std::vector<hpx::util::locality_result> res2;
                BOOST_FOREACH(hpx::util::remote_locality_result const& r1, res.second)
                {
                    res2.push_back(r1);
                }

                BOOST_FOREACH(hpx::id_type id, hpx::util::locality_results(res2))
                {
                    antibodies_.push_back(id);
                }

            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, spawn_antibodies);

            /*std::tuple<bool, hpx::id_type> get_target(hpx::id_type antibody)
            {}*/

            //Delete Superfluous antibodies
            void kill_antibodies()
            {
            }
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
}

 ////////////////////////////////////////////////////////

typedef immune_system::server::antibodies_factory abf_type;

HPX_REGISTER_ACTION_DECLARATION(abf_type::init_abf_action,
    abf_init_abf_action
    );

HPX_REGISTER_ACTION_DECLARATION(abf_type::spawn_antibody_action,
    abf_spawn_antibody_action
    );

HPX_REGISTER_ACTION_DECLARATION(abf_type::spawn_antibodies_action,
    abf_spawn_antibodies_action
    );

HPX_REGISTER_ACTION_DECLARATION(abf_type::alien_factory_active_action,
    abf_alien_factory_active_action
    );



#endif //IMMUNE_SYSTEM_ANTIBODIES_FACTORY_HPP
