//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef IMMUNE_SYSTEM_FOREIGN_BODIES_HPP
#define IMMUNE_SYSTEM_FOREIGN_BODIES_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/actions/component_action.hpp>

//#include <immune_system/immune_system/server/foreign_bodies_factory.hpp>
//#include <immune_system/immune_system/server/antibodies_factory.hpp>
//#include <immune_system/server/antibodies.hpp>

//#include <immune_system/server/antibodies.hpp>


namespace immune_system
{
    namespace server
    {
        struct aliens_factory; 

        struct aliens
            : hpx::components::simple_component_base<aliens>
        {
            // send signal to alien_factory to create aliens
            // if no antibodies contact 7*spawn_rate; 
            // if 6 antibodies contact: (7-6) * spawn_rate;
            // Total spawn_rate_total > threshold val, send signal to 
            // 		alien_factory. 
            // If 7 antibodies in contact, send no signal to alien_factory
            // If 7 antibodies contact an alien for a set amount of time, 
            //		all the entities are destroyed. 

            aliens(){};
            ~aliens(){};

            aliens(hpx::id_type my_id)
                :my_id_(my_id)
                , t_(hpx::util::high_resolution_timer::high_resolution_timer())
            {
            }

            bool ab_connect(hpx::id_type antibody)
            {               
                hpx::id_type invalid_type;
                if (ab_ != invalid_type)
                    return false;
                else
                {
                    ab_ = antibody;
                    return true;
                }
            }

            HPX_DEFINE_COMPONENT_ACTION(aliens, ab_connect);

            ///////////////////////////////////////////////////

            bool ab_attached()
            {
                hpx::id_type invalid_type;
                if (ab_ != invalid_type)
                    return true;
                else
                    return false;
            }

            ///////////////////////////////////////////////////

            bool alien_active()
            {
                return true;
            }

            HPX_DEFINE_COMPONENT_ACTION(aliens, alien_active);

            //////////////////////////////////////////////////
            

            // Single Server Factory Model.
            // If there are already foreign objects attached
            std::size_t send_spawn_signal()
            {
                return 0;
            }
            HPX_DEFINE_COMPONENT_ACTION(aliens, send_spawn_signal);

            // Each alien (bacteria) reproduces by cell division
//             hpx::id_type fission()
//             {
//                 return ;
//             }

//            HPX_DEFINE_COMPONENT_ACTION(aliens, fission);
        private:
            hpx::id_type my_id_;
            hpx::id_type ab_;
            //std::vector<hpx::id_type> ab_contact_;
            hpx::util::high_resolution_timer t_;
        };

    }
}
//////////////////////////////////////////////////////////////////

typedef immune_system::server::aliens aliens_type;

HPX_REGISTER_ACTION_DECLARATION(aliens_type::ab_connect_action
    , aliens_ab_connect_action);

HPX_REGISTER_ACTION_DECLARATION(aliens_type::alien_active_action
    , aliens_alien_active_action);

HPX_REGISTER_ACTION_DECLARATION(aliens_type::send_spawn_signal_action
    , aliens_send_spawn_signal_action);

#endif //IMMUNE_SYSTEM_FOREIGN_BODIES_HPP