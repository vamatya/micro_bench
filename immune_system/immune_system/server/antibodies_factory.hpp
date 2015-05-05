//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef IMMUNE_SYSTEM_SERVER_ANTIBODIES_FACTORY_HPP
#define IMMUNE_SYSTEM_SERVER_ANTIBODIES_FACTORY_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/actions/component_action.hpp>
//#include <hpx/runtime/components/migrate_component.hpp>
#include <hpx/util/tuple.hpp>

#include <immune_system/immune_system/server/foreign_bodies_factory.hpp>
#include "antibodies.hpp"
#include "common_objects.hpp"


namespace immune_system{ namespace server
{
    struct alien_factory;

    // Foreign Bodies Detection and Generating Antibodies. 
    // Maintain State of objects created
    struct antibodies_factory
        : hpx::components::simple_component_base<antibodies_factory>
    {
        typedef hpx::util::tuple<bool, hpx::id_type> tup_type;
        // old, new
        typedef hpx::util::tuple<hpx::id_type, hpx::id_type> old_new_tup_type;

        antibodies_factory()
        {
            time_.restart();
        }

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
//             if (al_factory_ != invalid_type)
//             {
//                 typedef immune_system::server::alien_factory::aliens_active_action
//                     action_type;
//                 hpx::future<bool> ret_type = hpx::async<action_type>(al_factory_);
// 
//                 return boost::move(ret_type.get());
//             }
//             else
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
            typedef std::vector<hpx::id_type> id_vector_type;
            typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;

            id_vector_type ret_res;

            if (num > 0)
            {

//                 hpx::components::component_type c_type =
//                     hpx::components::get_component_type <
//                     immune_system::server::antibodies > ();

                hpx::id_type this_loc = hpx::find_here();
                ret_res.reserve(num);

                hpx::future<std::vector<hpx::id_type> > fut =
                    hpx::new_<immune_system::server::antibodies[]>(
                        this_loc, num);

                ret_res = fut.get();
            }
            return ret_res;
        }

        HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, spawn_n_antibodies);

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
                        antibodies_.size()*std::pow(
                            2.0, time_.elapsed_microseconds() / 200000000));

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
                my_id_, ab_num_create);

            std::vector<hpx::future<void> > vec_fut;
            typedef immune_system::server::antibodies::init_action
                    init_action_type;

            std::vector<hpx::id_type> vec_id = f_vec_id.get();

            tup_type temp;
            //TODO: faster version?
            std::size_t cnt = vec_id.size();
                
            //TO DO: Revisit this section
            for (hpx::id_type& id : vec_id)
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

            // Migrate Antibodies to localities where aliens are located

            for (bodies& bd : antibodies_)
            {
                if((bd.foreign_object_attached) && (!bd.migrated))
                {
                       
                    hpx::id_type remote_loc = 
                        hpx::naming::get_locality_from_id(
                            bd.foreign_object);
                    hpx::id_type my_loc = 
                        hpx::naming::get_locality_from_id(
                            bd.my_id);
                    if (remote_loc != my_loc)
                    {
                        hpx::future<hpx::id_type> fut = 
                            hpx::components::migrate<
                            immune_system::server::antibodies>
                                (bd.my_id, remote_loc);
                    }
                            
                }
            }

            if(migration_futures_.size() != NULL)
                hpx::wait_all(migration_futures_);
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
        std::size_t max_antibodies_per_loc_;
        std::size_t max_aliens_per_loc_;
        std::size_t num_localities_;

        std::vector<hpx::future<hpx::id_type> > migration_futures_;
        std::vector<hpx::id_type> antibody_factories_;
        std::vector<hpx::id_type> alien_factories_; 
        std::vector<bodies> antibodies_;
        std::vector<old_new_tup_type> ab_after_migration_;

        hpx::util::high_resolution_timer time_;
    };
}}

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

#endif //IMMUNE_SYSTEM_ANTIBODIES_FACTORY_HPP
