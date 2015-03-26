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
//#include <immune_system/immune_system/server/foreign_bodies_factory.hpp>
#include "common_objects.hpp"
#include "foreign_bodies_factory.hpp"
//#include "foreign_bodies.hpp"
//#include "antibodies_factory.hpp"
//#include <immune_system/immune_system/server/foreign_bodies.hpp>

//struct immune_system::server::alien_factory;

namespace immune_system {
    namespace server
    {
        //struct alien_factory;

        struct antibodies
            : hpx::components::migration_support<
                hpx::components::simple_component_base<antibodies>
            >
        {
            typedef hpx::util::tuple<bool, hpx::id_type> tup_type;

            // Contact the Antibody targeted for, (error case). 
            // If the alien already has all the contact points filled, 
            // contact ab_factory for possible new target, or (self destroy??)
            // or get terminated??
            antibodies(){}

            // Explicitly destroy superfluous antibodies. 
            // Done by the antibodies_factory. 
            ~antibodies(){}

            antibodies(hpx::id_type my_id)
                : origin_loc_id_(hpx::find_here())
            {
                body_.my_id = my_id;
                body_.foreign_object_attached = false;
            }

            // look for target as soon as antibody is created.
            antibodies(hpx::id_type target, hpx::id_type my_id)
                : target_factory_(target)
                , origin_loc_id_(hpx::find_here())
            {
                body_.foreign_object_attached = false;
                body_.my_id = my_id;

                hpx::id_type invalid_type;

                //if(!connect.get())
                {
                    //notify antibodies_factory that connection failed
                    //typedef components::antibodies_factory::noconnect_notify_action 
                    //action_type;
                }
            }

            hpx::util::tuple<bool, hpx::id_type>                 // return alien_id, if connection successful
                alien_connect_helper(hpx::id_type target_factory)//,hpx::id_type my_id)
            {
                hpx::future<tup_type> fut_tup;

                typedef
                    immune_system::server::alien_factory::alien_connect_action
                    action_type;

                fut_tup = hpx::async<action_type>(target_factory, body_.my_id);
                return fut_tup.get();
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies, alien_connect_helper);

            bool      
            alien_connect(hpx::id_type target_factory)
            {
                // update target. 
                target_factory_ = target_factory;
                //

                //typedef std::tuple<bool, hpx::id_type> tup_type;
                hpx::future<tup_type> tup_fut;

                tup_type res_pair = alien_connect_helper(target_factory_);// , my_id_);
                //hpx::util::get
                if (!hpx::util::get<0>(res_pair))
                    return false;
                else
                {
                    body_.foreign_object = hpx::util::get<1>(res_pair);
                    body_.foreign_object_attached = true;                    
                    return true;
                }
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies, alien_connect);

//             template <typename Archive>
//             void serialize(Archive&ar, unsigned version) {}

        private:
            bodies body_;
            //hpx::id_type my_id_;
            hpx::id_type target_factory_;
            //hpx::id_type target_alien_;

            hpx::id_type origin_loc_id_;
            //bool alien_attached_;

            tup_type tup_;

            std::size_t origin_loc_rank_;
            std::size_t gen_id;
            //std::size_t fb_contact_count;

            friend class boost::serialization::access;
            template<class Archive>
            void serialize(Archive & ar, const unsigned int version){
                ar & my_id_;
                ar & tup_;
//                 ar & t_;
//                 ar & t_max_reached_;
            }
        };
    }
}

//////////////////////////////////////////////////////////////////

HPX_REGISTER_ACTION_DECLARATION(
    immune_system::server::antibodies::alien_connect_action
    , antibodies_alien_connect_action
    )

HPX_REGISTER_ACTION_DECLARATION(
    immune_system::server::antibodies::alien_connect_helper_action
    , antibodies_alien_connect_helper_action
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
