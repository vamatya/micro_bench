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

namespace immune_system {
    namespace server
    {

        struct antibodies
            : hpx::components::managed_component_base < antibodies >
        {

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
                
            }

        private:
            hpx::id_type my_id_;
            hpx::id_type target_;
            bool target_attached;

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

#endif //IMMUNE_SYSTEM_SERVER_ANTIBODIES_HPP
