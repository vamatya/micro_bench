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
#include <hpx/util/tuple.hpp>

#include <immune_system/immune_system/server/foreign_bodies_factory.hpp>
#include "antibodies.hpp"
#include "common_objects.hpp"


namespace immune_system{
    namespace server
    {
        struct alien_factory;

        // Foreign Bodies Detection and Generating Antibodies. 
        // Maintain State of objects created
        struct antibodies_factory
            : hpx::components::simple_component_base<antibodies_factory>
        {
            typedef hpx::util::tuple<bool, hpx::id_type> tup_type;
            //Note: For now maintain list of all spawned antibodies. 

            //TO DO: create another header file.

            antibodies_factory()
            {
                time_.restart();
            }

//             antibodies_factory(hpx::id_type my_id, std::size_t lcl_num, std::size_t max_antibodies)
//                 : my_id_(my_id)
//                 , num_localities_(lcl_num)
//                 , max_antibodies_(max_antibodies)
//             {
// 
//             }

            void init_abf(std::vector<hpx::id_type> alien_factories
                , hpx::id_type my_id, std::size_t num_localities
                , std::size_t max_aliens_perloc)
            {

            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, init_abf);

            //////////////////////////////////////////////////////////////////////////

            void init_resolve_names(std::vector<hpx::id_type> alien_factories
                , hpx::id_type my_id, std::size_t num_localities
                , std::size_t max_aliens_perloc
                , std::vector<hpx::id_type> ids, std::size_t rank)
            {
                alien_factories_ = alien_factories;
                my_id_ = my_id;
                num_localities_ = num_localities;
                antibody_factories_ = ids;
                my_rank_ = rank;
                max_aliens_per_loc_ = max_aliens_perloc;
                max_antibodies_per_loc_ = max_aliens_perloc * alien_factories.size() / num_localities;
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, init_resolve_names);

            //Sense foreign body
            // Identify each alien body, and get the no. of antibodies it has 
            // in contact. 
            // alien_factory notifies its location/id
            bool alien_factory_active()
            {
                hpx::id_type invalid_type;
//                 if (al_factory_ != invalid_type)
//                 {
//                     typedef immune_system::server::alien_factory::aliens_active_action
//                         action_type;
//                     hpx::future<bool> ret_type = hpx::async<action_type>(al_factory_);
// 
//                     return boost::move(ret_type.get());
//                 }
//                 else
                    return true;
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, alien_factory_active);

            bool update_alien_factory(hpx::id_type al_factory)
            {
                //al_factory_ = al_factory;
                return true;
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, update_alien_factory);


            //Generate antibodies
            // For every contact point vaccant, generate an antibody and 
            // spawn an antibody, targetting that antibody.


            // look for identified aliens and target them. 
            // distributing factory method call components. 
           

            //Spawn \N Antibodies
            //template <typename Component>
            std::vector<hpx::id_type>
            spawn_n_antibodies(std::size_t num)
            {
                typedef hpx::util::remote_locality_result value_type;
                typedef std::pair<std::size_t, std::vector<value_type> > result_type;
                typedef std::vector<hpx::id_type> id_vector_type;
                typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;

                std::vector<hpx::id_type> ret_res;

                if (num > 0)
                {
                    result_type res;

                    hpx::components::component_type c_type =
                        hpx::components::get_component_type <
                        immune_system::server::antibodies > ();

                    hpx::id_type this_loc = hpx::find_here();

                    typedef
                        hpx::components::server::runtime_support::bulk_create_components_action
                        action_type;

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

                    //antibodies_.reserve(num);
                    ret_res.reserve(num);

                    std::vector<hpx::util::locality_result> res2;
                    BOOST_FOREACH(hpx::util::remote_locality_result const& r1, res.second)
                    {
                        res2.push_back(r1);
                    }


                    BOOST_FOREACH(hpx::id_type id, hpx::util::locality_results(res2))
                    {
                        ret_res.push_back(id);
                    }


                }
                return ret_res;
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, spawn_n_antibodies);

//             template <typename T>
//             struct spawn_n_antibodies_action
//                 : hpx::actions::make_action < void (antibodies_factory::*)(T),
//                 &antibodies_factory::template spawn_n_antibodies<T>, spawn_n_antibodies_action<T> >
//             {};

            std::size_t num_create()//hpx::util::high_resolution_timer& t)
            {

                std::size_t y = 0;
                if (time_.elapsed_microseconds() > 0) //!= hpx::util::high_resolution_timer::high_resolution_timer())
                {
                    // Exponential Function for Bacterial Growth
                    // P(t) = P(0)* power(2,t/1000)  
                    // population doubles every 10 mili-seconds

                    //BOOST_ASSERT(max_aliens_ > 0);
                    BOOST_ASSERT(max_antibodies_per_loc_ > 0);

                    //if (antibodies_.size() < max_aliens_)
                    if (antibodies_.size() < max_antibodies_per_loc_)
                    {
                        // input in step of microseconds. 
                        y = static_cast<std::size_t>(
                            antibodies_.size()*std::pow(2.0, time_.elapsed_microseconds() / 200000000));

                        //max_aliens_* std::exp(
                        //    -1 * std::exp(
                        //        -2 * static_cast<double>(t_.elapsed()/1000000))));
                    }
                }

                //if (antibodies_.size() + y < max_aliens_)
                if (antibodies_.size() + y < max_antibodies_per_loc_)
                    return y;
                else
                {
                    //if ((max_aliens_ - antibodies_.size()) > 0)
                    if ((max_antibodies_per_loc_ - antibodies_.size()) > 0)
                    {
                        //return (max_aliens_ - antibodies_.size());
                        return (max_antibodies_per_loc_ - antibodies_.size());
                    }
                    else{
                        return 0;
                    }
                }

            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, num_create);

            void target_aliens(hpx::id_type alien_factory)
            {
                typedef hpx::util::tuple< hpx::future<bool>, hpx::id_type > fut_tup_type;
                typedef std::vector<fut_tup_type> fut_vec_type;
                typedef immune_system::server::alien_factory::antibodies_deficit_action
                    deficit_action_type;
                typedef immune_system::server::antibodies::alien_connect_action
                    connect_action_type;
                typedef hpx::util::tuple<bool, hpx::id_type> tup_type;
                typedef hpx::future<tup_type> f_tup_type;
                
                BOOST_ASSERT(alien_factories_.size() != NULL);

                hpx::id_type invalid_type;

                hpx::future<std::size_t> f_deficit_num =
                    hpx::async<deficit_action_type>(alien_factory); 

                std::size_t deficit_num = boost::move(f_deficit_num.get());
                std::size_t ab_num_create = num_create();

                if (deficit_num)
                {
                    if (deficit_num > ab_num_create)
                    {
                        ab_num_create = deficit_num;
                    }
                }
                else
                {
                    ab_num_create = 0;
                }
                    
                // Create Antibodies
                typedef antibodies_factory::spawn_n_antibodies_action
                    spawn_antibodies_action_type;
                hpx::future<std::vector<hpx::id_type> > f_vec_id;
                f_vec_id = hpx::async<spawn_antibodies_action_type>(
                    //this->gid_,ab_num_create);
                    my_id_, ab_num_create);

                std::vector<hpx::future<void> > vec_fut;
                typedef immune_system::server::antibodies::init_action
                        init_action_type;

                std::vector<hpx::id_type> vec_id = f_vec_id.get();

                tup_type temp;
                //TODO: faster version?
                std::size_t cnt = vec_id.size();
                
                //TO DO: Revisit this section
                //while (cnt != 0)
                //{
                BOOST_FOREACH(hpx::id_type id, vec_id)
                {
                        
                    f_tup_type res_f = hpx::async<
                        connect_action_type>(id, id, alien_factory);
                    temp = res_f.get();
                    bodies bd(id);
                    if (hpx::util::get<0>(temp))
                    {
                        bd.foreign_object_attached
                            = hpx::util::get<0>(temp);
                        bd.foreign_object = hpx::util::get<1>(temp);
                        antibodies_.push_back(bd);
                    }
                    else
                    {
                        bodies bd(false, id);
                        bd.foreign_object_attached
                            = hpx::util::get<0>(temp);
                        antibodies_.push_back(bd);
                    }
                        
                }
                //}

            }
            
            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, target_aliens);
            //TO DO:
            // Terminate Antibodies Factory
            //Delete Superfluous antibodies
            //Antibodies Create Credit?
            void print_stat()
            {
                std::cout << "My Rank:" << my_rank_ << std::endl;
                std::cout << "Total Antibodies Created:" << antibodies_.size() << std::endl;
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, print_stat);
        private:
            hpx::id_type my_id_;
            std::size_t produced_count;
            std::size_t my_rank_;

            std::vector<hpx::id_type> antibody_factories_;
            std::vector<hpx::id_type> alien_factories_; //single factory for now
            std::vector<bodies> antibodies_;
            //std::vector<hpx::util::tuple<bool, hpx::id_type> > antibodies_;
            //std::vector<hpx::id_type> antibodies_;

            std::size_t max_antibodies_per_loc_;
            std::size_t max_aliens_per_loc_;
            std::size_t num_localities_;
            //std::size_t max_aliens_;

            hpx::util::high_resolution_timer time_;

        };
    }
}

 ////////////////////////////////////////////////////////

typedef immune_system::server::antibodies_factory abf_type;

HPX_REGISTER_ACTION_DECLARATION(abf_type::init_abf_action
    , abf_init_abf_action
    );
HPX_REGISTER_ACTION_DECLARATION(abf_type::alien_factory_active_action
    , abf_alien_factory_active_action
    );

HPX_REGISTER_ACTION_DECLARATION(abf_type::print_stat_action
    , abf_print_stat_action
    );
HPX_REGISTER_ACTION_DECLARATION(abf_type::update_alien_factory_action
    , abf_update_alien_factory_action);

HPX_REGISTER_ACTION_DECLARATION(abf_type::spawn_n_antibodies_action,
    abf_spawn_n_antibodies_action
    );

HPX_REGISTER_ACTION_DECLARATION(abf_type::init_resolve_names_action
    , abf_init_resolve_names_action
    );

HPX_REGISTER_ACTION_DECLARATION(abf_type::target_aliens_action,
    abf_target_aliens_action
    );
// HPX_REGISTER_ACTION_DECLARATION_TEMPLATE(
//     (template <typename T>),
//     (abf_type::spawn_n_antibodies_action<T>)
//     )

//HPX_REGISTER_ACTION_DECLARATION(abf_type::spawn_antibody_action,
//    abf_spawn_antibody_action
//    );





// HPX_REGISTER_ACTION_DECLARATION(abf_type::kill_antibodies_action,
//     abf_kill_antibodies_action
//     );


#endif //IMMUNE_SYSTEM_ANTIBODIES_FACTORY_HPP


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