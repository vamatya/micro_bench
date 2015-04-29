//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef IMMUNE_SYSTEM_SERVER_ANTIBODIES_HPP
#define IMMUNE_SYSTEM_SERVER_ANTIBODIES_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/actions/component_action.hpp>

#include "common_objects.hpp"
#include "foreign_bodies_factory.hpp"

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
            antibodies()
                :origin_loc_id_(hpx::find_here())
            {
                //body_.foreign_object_attached = false;
            }

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
            }

            void init(hpx::id_type my_id)
            {
                body_.my_id = my_id;
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies, init);

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

            // Overload?
            tup_type
            alien_connect(hpx::id_type my_id, hpx::id_type target_factory)
            {
                // update target. 
                target_factory_ = target_factory;
                body_.my_id = my_id;


                hpx::future<tup_type> tup_fut;
                typedef
                    immune_system::server::alien_factory::alien_connect_action
                    action_type;

                tup_fut = hpx::async<action_type>(target_factory, body_.my_id);

                tup_type res_pair = tup_fut.get();

                //ERROR ?!
                if (!hpx::util::get<0>(res_pair))
                {
                    return res_pair;
                }
                else
                {
                    body_.foreign_object = hpx::util::get<1>(res_pair);
                    body_.foreign_object_attached = true;                    
                    return res_pair;
                }
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies, alien_connect);

        private:
            bodies body_;
            hpx::id_type target_factory_;

            hpx::id_type origin_loc_id_;
            //bool alien_attached_;

            tup_type tup_;

            std::size_t origin_loc_rank_;
            std::size_t gen_id;

//             friend class boost::serialization::access;
//             template<class Archive>
//             void serialize(Archive & ar, const unsigned int version){
//                 ar & my_id_;
//                 ar & tup_;
// //                 ar & t_;
// //                 ar & t_max_reached_;
//             }
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

HPX_REGISTER_ACTION_DECLARATION(
    immune_system::server::antibodies::init_action
    , antibodies_init_action
)

#endif //IMMUNE_SYSTEM_SERVER_ANTIBODIES_HPP
