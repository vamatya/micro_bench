//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef IMMUNE_SYSTEM_SERVER_ANTIBODIES_HPP
#define IMMUNE_SYSTEM_SERVER_ANTIBODIES_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>

//#include <immune_system/server/antibodies_factory.hpp>
//#include <immune_system/server/foreign_bodies_factory.hpp>


namespace immune_system {
    namespace server
    {

        struct antibodies
            : hpx::components::managed_component_base < antibodies >
        {

            // Contact the Antibody targeted for, (error case). 
            // If the alien already has all the contact points filled, 
            // contact ab_factory for possible new target, or (self destroy??)
            // or get terminated??
            antibodies(){}

            // Explicitly destroy superfluous antibodies. 
            // Done by the antibodies_factory. 
            ~antibodies(){}

            // look for target as soon as antibody is created.
            antibodies(hpx::id_type target, hpx::id_type my_id)
                : target_attached(false)
                , target_(target)
                , my_id_(my_id)
            {
                //
                //typedef ::components::aliens::ab_connect_action action_type;
                //hpx::future<bool> connect = hpx::async<action_type>(target);

                //if(!connect.get())
                {
                    //notify antibodies_factory that connection failed
                    //typedef components::antibodies_factory::noconnect_notify_action 
                    //action_type;
                }
            }

            bool alien_connect(hpx::id_type target)
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

            HPX_DEFINE_COMPONENT_ACTION(antibodies, alien_connect);

            bool send_spawn_signal()
            {
                //typedef ::components::antibodies_factory::spawn_antibodies_action
                //    act_type;
                //typedef ::components::alien_factory::spawn_action act_type2;
                return true;
            }

        private:
            hpx::id_type my_id_;
            hpx::id_type target_;
            bool target_attached;

            std::size_t origin_loc_id_;
            std::size_t gen_id;
            std::size_t fb_contact_count;

        };
    }
}

//////////////////////////////////////////////////////////////////

HPX_REGISTER_ACTION_DECLARATION(
    immune_system::server::antibodies::alien_connect_action
    , antibodies_alien_connect_action
    )

/*HPX_REGISTER_ACTION_DECLARATION(
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
    */
#endif //IMMUNE_SYSTEM_SERVER_ANTIBODIES_HPP
