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

#include <cmath>
#include <iostream>
#include <cstdint>

namespace immune_system {
    namespace server
    {
        // Foreign(Alien) Body Generating Factory. 

        struct alien_factory
            : hpx::components::managed_component_base < alien_factory >
        {
            //Note: For now maintain list of all spawned aliens. 

            // Generate Aliens
            // Based on the feedback from initial alien created, generate 
            // more aliens. 
            //void spawn_n_aliens(std::size_t num);

            alien_factory(){}
            ~alien_factory(){}

            alien_factory(hpx::id_type my_id)
                : my_id_(my_id)
                , t_(hpx::util::high_resolution_timer::high_resolution_timer())
                , epsilon_(1000)
                , t_max_reached_(false,0)
            {
                spawn_n_aliens(1);
            }

            void init()
            {

            }
            
            HPX_DEFINE_COMPONENT_ACTION(alien_factory, init);


            void spawn_aliens()

            {
            }
//             {
//             hpx::components::component_type c_type =
//             hpx::components::get_component_type<components::aliens>();
// 
//             hpx::id_type this_locality = hpx::find_here(); //??
// 
//             using hpx::components::distributing_factory;
// 
//             typedef
//             hpx::components::server::runtime_support::bulk_create_components_action
//             action_type;
// 
//             typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;
// 
//             if(this_locality)
//             {
//             future_type f;
//             {
//             hpx::lcos::packaged_action<action_type
//             , std::vector<hpx::naming::gid_type> > p;
//             p.apply(hpx::launch::async, this_locality, c_type, 1);
//             f = p.get_future();
//             }
//             ////converting gid_type to id_type??
//             //aliens_.push_back(f.get());
//             }
// 
//             //typedef ::components::aliens::ab_connect_action act_type;
// 
//             }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, spawn_aliens);

            void spawn_n_aliens(std::size_t num)
            {
                hpx::components::component_type c_type =
                    hpx::components::get_component_type<immune_system::server::aliens>();
                hpx::id_type this_locality = hpx::find_here();
                using hpx::components::distributing_factory;

                typedef
                hpx::components::server::runtime_support::bulk_create_components_action
                action_type;

                typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;

                future_type f;
                {
                    hpx::lcos::packaged_action<action_type
                        , std::vector<hpx::naming::gid_type> > p;
                    p.apply(hpx::launch::async, this_locality, c_type, num);
                    f = p.get_future();
                }
                aliens_.push_back(f.get());
            }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, spawn_n_aliens);

            // Returns no. of antibodies to spawn. 
            std::size_t num_create()//hpx::util::high_resolution_timer& t)
            {

                std::size_t y = 0;
                //if (t_ != hpx::util::high_resolution_timer::high_resolution_timer())
                {
                    // Exponential Function for Bacterial Growth
                    // P(t) = P(0)* power(2,t/1000)  
                    // population doubles every 10 mili-seconds
                
                    if (aliens_.size() < max_aliens_)
                    {
                        // input in step of microseconds. 
                        y = static_cast<std::size_t>(
                            aliens_.size()*std::pow(2.0, t_.elapsed_microseconds() / 10000));

                            //max_aliens_* std::exp(
                            //    -1 * std::exp(
                            //        -2 * static_cast<double>(t_.elapsed()/1000000))));
                    }
                }

                if (aliens_.size() + y < max_aliens_)
                    return y;
                else 
                {
                    if ((max_aliens_ - aliens_.size()) > 0)
                        return (max_aliens_ - aliens_.size());
                    else
                        return 0;
                }
                    
            }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, num_create);


            bool aliens_active()
            {
                // spawn aliens till set max has been reached 
                // if antibodies_count closely matches aliens count for 10 seconds, stop
                if ((aliens_.size() == max_aliens_) &&
                    (aliens_.size() - antibodies_.size() < epsilon_) &&
                    std::get<0>(t_max_reached_) &&
                    ((t_.elapsed_microseconds() - std::get<1>(t_max_reached_)) > 10000000))
                    return true;
                else
                    return false;
            }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, aliens_active);

            void create_aliens()
            {
                while (aliens_active())
                {
                    std::size_t aliens_to_spawn = num_create();
                    
                    // Max limit of aliens to spawn reached
                    if (aliens_to_spawn = 0)
                    {
                        if (!std::get<0>(t_max_reached_))
                        {
                            std::get<0>(t_max_reached_) = true;
                            std::get<1>(t_max_reached_) = t_.elapsed_microseconds();
                        }
                    }
                    else
                        spawn_n_aliens(aliens_to_spawn);
                }
            }
            
            HPX_DEFINE_COMPONENT_ACTION(alien_factory, create_aliens);
            // Aliens Create Credit?
        private:
            std::size_t epsilon_;
            std::size_t max_aliens_;
            hpx::id_type my_id_;
            // tuple??
            std::vector<hpx::id_type> aliens_;
            std::vector<hpx::id_type> antibodies_;
            hpx::util::high_resolution_timer t_;
            std::tuple<bool, std::uint64_t> t_max_reached_;
            
        };
    }
}
//////////////////////////////////////////////////////////////////

typedef immune_system::server::alien_factory af_type;

HPX_REGISTER_ACTION_DECLARATION(af_type::spawn_aliens_action
    , alien_factory_spawn_action);

HPX_REGISTER_ACTION_DECLARATION(af_type::spawn_n_aliens_action
    , alien_factory_spawn_n_aliens_action);

#endif //IMMUNE_SYSTEM_FOREIGN_BODIES_FACTORY_HPP