//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef IMMUNE_SYSTEM_FOREIGN_BODIES_HPP
#define IMMUNE_SYSTEM_FOREIGN_BODIES_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/actions/component_action.hpp>

#include <immune_system/immune_system/server/foreign_bodies_factory.hpp>
//#include <immune_system/immune_system/server/antibodies_factory.hpp>
//#include <immune_system/server/antibodies.hpp>

//#include <immune_system/server/antibodies.hpp>


namespace immune_system
{
    namespace server
    {
        
        struct aliens
            : hpx::components::managed_component_base < aliens >
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
            {

            }

            bool ab_connect(hpx::id_type antibody)
            {
                if (antibody)
                {
                    if (ab_contact_.size() < 7)
                    {
                        ab_contact_.push_back(antibody);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }

            HPX_DEFINE_COMPONENT_ACTION(aliens, ab_connect);

            ///////////////////////////////////////////////////

            bool alien_active()
            {
                //typedef ::components::alien_factory::spawn_action act_type;
                //typedef ::components::antibodies_factory::spawn_antibodies_action act_type2;

                return true;
            }

            HPX_DEFINE_COMPONENT_ACTION(aliens, alien_active);


            //////////////////////////////////////////////////
            std::size_t send_spawn_signal()
            {
                //typedef ::components::alien_factory::spawn_action act_type;
                //typedef ::components::antibodies_factory::spawn_antibodies_action
                //act_type2;
                //typedef ::components::antibodies::alien_connect_action act_type3;

            }
            HPX_DEFINE_COMPONENT_ACTION(aliens, send_spawn_signal);

        private:
            hpx::id_type my_id_;
            std::vector<hpx::id_type> ab_contact_;
        };

    }
}
//////////////////////////////////////////////////////////////////

HPX_REGISTER_ACTION_DECLARATION(
    ::components::aliens::ab_connect_action,
    aliens_ab_connect_action
    );

HPX_REGISTER_ACTION_DECLARATION(
    ::components::aliens::alien_active_action,
    aliens_alien_active_action
        );

HPX_REGISTER_ACTION_DECLARATION(
    ::components::aliens::send_spawn_signal_action,
    aliens_send_spawn_signal_action
        );

#endif //IMMUNE_SYSTEM_FOREIGN_BODIES_HPP