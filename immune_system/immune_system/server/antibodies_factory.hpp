//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef IMMUNE_SYSTEM_SERVER_ANTIBODIES_FACTORY_HPP
#define IMMUNE_SYSTEM_SERVER_ANTIBODIES_FACTORY_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>

#include <hpx/components/distributing_factory/distributing_factory.hpp>


#include <immune_system/immune_system/server/foreign_bodies_factory.hpp>
#include "antibodies.hpp"


namespace immune_system{
    namespace server
    {
        struct alien_factory;

        // Foreign Bodies Detection and Generating Antibodies. 
        // Maintain Stat of objects created
        struct antibodies_factory
            : hpx::components::simple_component_base<antibodies_factory>
        {
            typedef hpx::util::tuple<bool, hpx::id_type> tup_type;
            //Note: For now maintain list of all spawned antibodies. 

            //TO DO: create another header file.

            antibodies_factory(){}

            antibodies_factory(hpx::id_type my_id, std::size_t lcl_num, std::size_t max_antibodies)
                : my_id_(my_id)
                , num_localities_(lcl_num)
                , max_antibodies_(max_antibodies)
            {

            }

            void init_abf()
            {
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, init_abf);

            //////////////////////////////////////////////////////////////////////////

            void resolve_names(std::vector<hpx::id_type> ids, std::size_t rank)
            {
                fac_ids_ = ids;
                my_rank_ = rank;
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

//             bool scan_aliens()
//             {
//                 return false;
//             }

            //Generate antibodies
            // For every contact point vaccant, generate an antibody and 
            // spawn an antibody, targetting that antibody.


            // look for identified aliens and target them. 
            // distributing factory method call components. 
            //void spawn_antibody()
            //{
            //typedef ::components::alien_factory::spawn_action action_type;
            //} 
            //HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, spawn_antibody);

            //Spawn \N Antibodies
            template <typename Component>
            void spawn_n_antibodies(std::size_t num)
            {
                typedef hpx::util::remote_locality_result value_type;
                typedef std::pair<std::size_t, std::vector<value_type> > result_type;

                result_type res;


                typedef std::vector<hpx::id_type> id_vector_type;

                hpx::components::component_type c_type =
                    hpx::components::get_component_type<Component>();

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
                res.second.push_back(
                    value_type(this_loc.get_gid(), c_type));
                res.second.back().gids_ = boost::move(f.get());

                antibodies_.reserve(num);

                std::vector<hpx::util::locality_result> res2;
                BOOST_FOREACH(hpx::util::remote_locality_result const& r1, res.second)
                {
                    res2.push_back(r1);
                }

                BOOST_FOREACH(hpx::id_type id, hpx::util::locality_results(res2))
                {
                    antibodies_.push_back(hpx::util::make_tuple(false, id));
                }

            }

            //HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, spawn_n_antibodies);

            template <typename T>
            struct spawn_n_antibodies_action
                : hpx::actions::make_action < void (antibodies_factory::*)(T),
                &antibodies_factory::template spawn_n_antibodies<T>, spawn_n_antibodies_action<T> >
            {};


            void target_aliens()
            {
                BOOST_ASSERT(fac_ids_.size() != NULL);
                BOOST_ASSERT(antibodies_.size() != NULL);

                hpx::id_type invalid_type;

                //result, ab_id 
                typedef hpx::util::tuple<hpx::future<bool>, hpx::id_type> fut_tup_type;

                typedef std::vector<fut_tup_type> fut_vec_type;
                
                fut_vec_type fvec;
                
                fut_tup_type temp_tup;

                typedef immune_system::server::antibodies::alien_connect_action
                    action_type;

//                 BOOST_FOREACH(hpx::id_type alien_factory, fac_ids_)
//                 {
//                     BOOST_FOREACH(hpx::id_type id, antibodies_)
//                     {
//                         std::get<0>(temp_tup) = hpx::async<action_type>(id, alien_factory)
//                     }
//                 }
                BOOST_ASSERT(al_factory_ != invalid_type);

//                 BOOST_FOREACH(tup_type tup, antibodies_)
//                 {
//                     hpx::util::get<0>(temp_tup) 
//                         = hpx::async<action_type>(hpx::util::get<1>(tup), al_factory_);
//                     hpx::util::get<1>(temp_tup) = hpx::util::get<1>(tup);
//                     fvec.push_back(temp_tup);
//                 }
                std::vector<hpx::future<bool> > ret_vec;
                BOOST_FOREACH(hpx::id_type id, antibodies_)
                {
                    ret_vec.push_back(
                        hpx::async<action_type>(id, al_factory_));
                }

                hpx::wait_all(ret_vec);

                //fut_vec_type::iterator itr_f = fvec.begin();
            }
            
            //Delete Superfluous antibodies
//             void kill_antibodies(std::vector<hpx::id_type> abs)
//             {
//                 BOOST_FOREACH(hpx::id_type id, abs)
//                 {
//                     std::size_t count = 0;
//                     bool found = false;
//                     //std::size_t 
//                     BOOST_FOREACH(hpx::id_type id_fac, antibodies_)
//                     {
//                         if (id_fac == id)
//                         {
//                             found = true;
//                             break;
//                         }
//                         ++count;
//                     }
// 
//                     if (found)
//                     {
//                         antibodies_.erase(antibodies_.begin()+count);
//                     }
//                 }
//             }
// 
//             HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, kill_antibodies);
// 
//             void kill_antibody(hpx::id_type ab)
//             {
//             }
//             //spawn_fb??
// 
            //Antibodies Create Credit?
            void print_stat()
            {
                std::cout << "My Rank:" << my_rank_ << std::endl;
                std::cout << "Total Antibodies Created:" << antibodies_.size() << std::endl;
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, print_stat);
        private:
            hpx::id_type my_id_;
            hpx::id_type al_factory_;   // single factory for now
            std::size_t produced_count;
            std::size_t my_rank_;

            std::vector<hpx::id_type> fac_ids_;
            //std::vector<hpx::util::tuple<bool, hpx::id_type> > antibodies_;
            std::vector<hpx::id_type> antibodies_;

            std::size_t max_antibodies_;
            std::size_t num_localities_;
        };
    }
}

 ////////////////////////////////////////////////////////

typedef immune_system::server::antibodies_factory abf_type;

HPX_REGISTER_ACTION_DECLARATION(abf_type::init_abf_action,
    abf_init_abf_action
    );

//HPX_REGISTER_ACTION_DECLARATION(abf_type::spawn_antibody_action,
//    abf_spawn_antibody_action
//    );

// HPX_REGISTER_ACTION_DECLARATION(abf_type::spawn_n_antibodies_action,
//     abf_spawn_n_antibodies_action
//     );

HPX_REGISTER_ACTION_DECLARATION(abf_type::alien_factory_active_action,
    abf_alien_factory_active_action
    );

HPX_REGISTER_ACTION_DECLARATION(abf_type::print_stat_action,
    abf_print_stat_action
    );

// HPX_REGISTER_ACTION_DECLARATION(abf_type::kill_antibodies_action,
//     abf_kill_antibodies_action
//     );

HPX_REGISTER_ACTION_DECLARATION_TEMPLATE(
    (template <typename T>),
    (abf_type::spawn_n_antibodies_action<T>)
    )

#endif //IMMUNE_SYSTEM_ANTIBODIES_FACTORY_HPP
