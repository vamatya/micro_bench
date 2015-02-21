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
            //typedef std::tuple<bool, hpx::id_type> tup_type;
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
                : my_id_(my_id)
                , origin_loc_id_(hpx::find_here())
                , alien_attached_(false)
            {}

            // look for target as soon as antibody is created.
            antibodies(hpx::id_type target, hpx::id_type my_id)
                : alien_attached_(false)
                , target_factory_(target)
                , my_id_(my_id)
                , origin_loc_id_(hpx::find_here())
            {
                //typedef ::components::aliens::ab_connect_action action_type;
                //hpx::future<bool> connect = hpx::async<action_type>(target);
                
                hpx::id_type invalid_type;

//                typedef 
//                    immune_system::server::alien_factory::alien_connect_action
//                    action_type;

               typedef
                   immune_system::server::aliens::send_spawn_signal_action
                   action_type;

//                typedef
//                immune_system::server::alien_factory::spawn_n_aliens_action
//                              action_type2;
               typedef
               immune_system::server::alien_factory::alien_connect_action
               action_type1;

//                typedef
//                    immune_system::server::antibodies_factory::alien_factory_active_action
//                    action_type5;

                //if(!connect.get())
                {
                    //notify antibodies_factory that connection failed
                    //typedef components::antibodies_factory::noconnect_notify_action 
                    //action_type;
                }
            }

            hpx::util::tuple<bool, hpx::id_type>                      // return alien_id, if connection succesfull
                alien_connect_helper(hpx::id_type target_factory)//,hpx::id_type my_id)
            {
                hpx::future<tup_type> fut_tup;

                typedef
                    immune_system::server::alien_factory::alien_connect_action
                    action_type;

                fut_tup = hpx::async<action_type>(target_factory, my_id_);
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
                    target_alien_ = hpx::util::get<1>(res_pair);
                    alien_attached_ = true;
                    return true;
                }
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies, alien_connect);

//             template <typename Archive>
//             void serialize(Archive&ar, unsigned version) {}

        private:
            hpx::id_type my_id_;
            hpx::id_type target_factory_;
            hpx::id_type target_alien_;

            hpx::id_type origin_loc_id_;
            bool alien_attached_;

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
