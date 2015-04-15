//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef IMMUNE_SYSTEM_FOREIGN_BODIES_FACTORY_HPP
#define IMMUNE_SYSTEM_FOREIGN_BODIES_FACTORY_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/components/distributing_factory/distributing_factory.hpp>
#include <hpx/util/tuple.hpp>

#include "common_objects.hpp"
#include "foreign_bodies.hpp"

#include <cmath>
#include <iostream>
#include <cstdint>

//TODO Thread Safety

namespace immune_system {
    namespace server
    {
        // Foreign(Alien) Body Generating Factory. 

        struct alien_factory
            : hpx::components::simple_component_base<alien_factory>
        {

            typedef hpx::lcos::local::spinlock mutex_type;

            //typedef std::tuple<bool, hpx::id_type> tup_type;
            typedef hpx::util::tuple<bool, hpx::id_type> tup_type;

            //Note: For now maintain list of all spawned aliens. 

            // Generate Aliens
            // Based on the feedback from initial alien created, generate 
            // more aliens. 
            //void spawn_n_aliens(std::size_t num);

            alien_factory()
                : time_(hpx::util::high_resolution_timer::high_resolution_timer())
                , epsilon_(1000)
                , t_max_reached_(false,0)
                , anti_body_count_(0)
            {
                spawn_n_aliens(1);
            }
            ~alien_factory(){}

//             alien_factory(hpx::id_type my_id)
//                 : my_id_(my_id)
//                 , time_(hpx::util::high_resolution_timer::high_resolution_timer())
//                 , epsilon_(1000)
//                 , t_max_reached_(false,0)
//                 , anti_body_count_(0)
//             {
//                 spawn_n_aliens(1);
//             }
            void init(hpx::id_type my_id, std::size_t max_alien_num)
            {
                my_id_ = my_id;
                max_aliens_ = max_alien_num;                
            }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, init);

            
//             bool alien_connect_helper(hpx::id_type ab_id)
//             {
//                 typedef immune_system::server::aliens::
//                 hpx::future<bool> res = hpx:
//             }

            // Individual antibody contact AF
            tup_type alien_connect(hpx::id_type ab_id)
            {

                typedef immune_system::server::aliens::ab_connect_action
                    action_type;    
                hpx::id_type invalid_type;
                tup_type res_tup = hpx::util::make_tuple(false, invalid_type);
                //tup_type temp = std::make_tuple(false,invalid_type);

                hpx::future<bool> res;
                BOOST_FOREACH(bodies b, aliens_)
                {
                    if (!b.foreign_object_attached)
                    {
                        res = hpx::async<action_type>(b.my_id, ab_id);
                        BOOST_ASSERT(res.get() == true);
                        hpx::util::get<0>(res_tup) = res.get();
                        hpx::util::get<1>(res_tup) = b.my_id;
                        b.foreign_object = ab_id; 
                        ++anti_body_count_;
                        break;
                    }
                    
                }

                return res_tup;
            }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, alien_connect);

            void spawn_n_aliens(std::size_t num)
            {
                typedef hpx::util::remote_locality_result value_type;
                typedef std::pair<std::size_t, std::vector<value_type> > result_type;

                result_type res;

                hpx::components::component_type c_type =
                    hpx::components::get_component_type<immune_system::server::aliens>();
                hpx::id_type this_locality = hpx::find_here();
                using hpx::components::distributing_factory;

                typedef
                hpx::components::server::runtime_support::bulk_create_components_action
                action_type;

                typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;

                future_type f;
                {
                    hpx::lcos::packaged_action<action_type
                        , std::vector<hpx::naming::gid_type> > p;
                    p.apply(hpx::launch::async, this_locality, c_type, num);
                    f = p.get_future();
                }

                res.first = num;
                res.second.push_back(
                    value_type(this_locality.get_gid(), c_type));
                res.second.back().gids_ = boost::move(f.get());

                //id_vector_type comps;

                //comps.reserve(num);

                std::vector<hpx::util::locality_result> res2;
                BOOST_FOREACH(hpx::util::remote_locality_result const& r1, res.second)
                {
                    res2.push_back(r1);
                }

                typedef immune_system::server::aliens::init_action 
                    init_action_type;

                std::vector<hpx::future<void> > vec_fut;

                BOOST_FOREACH(hpx::id_type id, hpx::util::locality_results(res2))
                {
                    bodies temp(false, id);
                    aliens_.push_back(temp);
                    vec_fut.push_back(hpx::async<init_action_type>(id, id));
                }

//                 BOOST_FOREACH(hpx::id_type id, hpx::util::locality_results(res2))
//                 {
//                     tup_type temp = hpx::util::make_tuple(false, id);
//                     aliens_.push_back(temp);
//                 }

                hpx::wait_all(vec_fut);
            }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, spawn_n_aliens);

            // Returns no. of aliens to spawn. 
            std::size_t num_create()//hpx::util::high_resolution_timer& t)
            {

                std::size_t y = 0;
                if (time_.elapsed_microseconds() > 0) //!= hpx::util::high_resolution_timer::high_resolution_timer())
                {
                    // Exponential Function for Bacterial Growth
                    // P(t) = P(0)* power(2,t/1000)  
                    // population doubles every 10 mili-seconds

                    BOOST_ASSERT(max_aliens_ > 0);
                
                    if (aliens_.size() < max_aliens_)
                    {
                        // input in step of microseconds. 
                        y = static_cast<std::size_t>(
                            aliens_.size()*std::pow(2.0, time_.elapsed_microseconds() / 10000));
                
                            //max_aliens_* std::exp(
                            //    -1 * std::exp(
                            //        -2 * static_cast<double>(t_.elapsed()/1000000))));
                    }
                }

                if (aliens_.size() + y < max_aliens_)
                    return y;
                else 
                {
                    if ((max_aliens_ - aliens_.size()) > 0)
                        return (max_aliens_ - aliens_.size());
                    else
                        return 0;
                }
                    
            }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, num_create);

            // TODO: 
            void migrate_update_count()
            {
                mutex_type::scoped_lock lk(mtx);
                ++anti_body_count_;
            }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, migrate_update_count);


            bool aliens_active()
            {
                // spawn aliens till set max has been reached 
                // if antibodies_count closely matches aliens count for 10 seconds, stop

                //Check if the objects (anitbodies) are in the same locality or not...
                // update_abcount
                // Aliens Active until all the antibodies formed are also migrated to the 
                // locality where aliens are generated. 

                //update ab_count??
                if ((aliens_.size() == max_aliens_) && hpx::util::get<0>(t_max_reached_ ) == false)
                {
                    hpx::util::get<0>(t_max_reached_) = true;
                    hpx::util::get<1>(t_max_reached_) = time_.elapsed_microseconds();                    
                }

                //epsilon _ termination, lower limit?
                if ((aliens_.size() - anti_body_count_ < 10) && (aliens_.size() == max_aliens_))
                {  
                    if (time_.elapsed_microseconds() - hpx::util::get<1>(t_max_reached_) > 10000000)
                        return false;
                }                   
                    
                return true;
            }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, aliens_active);

            void create_aliens()
            {
                //while (aliens_active())
                if (aliens_active())
                {
                    std::size_t aliens_to_spawn = num_create();
                    if (aliens_to_spawn)
                    {
                        spawn_n_aliens(aliens_to_spawn);
                    }                    
                }
            }
            
            HPX_DEFINE_COMPONENT_ACTION(alien_factory, create_aliens);
            // Aliens Create Credit?

//             template <typename Archive>
//             void serialize(Archive&ar, unsigned version) {}

            std::size_t antibodies_deficit()
            {
                BOOST_ASSERT(aliens_.size() >= anti_body_count_);
                return aliens_.size() - anti_body_count_;
            }

            HPX_DEFINE_COMPONENT_ACTION(alien_factory, antibodies_deficit);

        private:
            std::size_t epsilon_;
            std::size_t max_aliens_;
            hpx::id_type my_id_;
            
            // tuple??
            tup_type tup_;
            std::vector<bodies> aliens_;

            // increasing update count of anti_body_migrated to this locality
            std::size_t anti_body_count_;
            
            //std::vector<tup_type> aliens_;
            std::size_t active_aliens_count;
            
            hpx::util::high_resolution_timer time_;
            hpx::util::tuple<bool, std::uint64_t> t_max_reached_;

            mutex_type mtx;

            friend class boost::serialization::access;
            template<class Archive>
            void serialize(Archive & ar, const unsigned int version){
                ar & aliens_;
                ar & tup_;
                ar & t_;
                ar & t_max_reached_;
            }
            
        };
    }
}
//////////////////////////////////////////////////////////////////

typedef immune_system::server::alien_factory af_type;

// HPX_REGISTER_ACTION_DECLARATION(af_type::spawn_aliens_action
//     , alien_factory_spawn_action);

HPX_REGISTER_ACTION_DECLARATION(af_type::spawn_n_aliens_action
    , alien_factory_spawn_n_aliens_action);

HPX_REGISTER_ACTION_DECLARATION(af_type::alien_connect_action
    , alien_factory_alien_connect_action);

HPX_REGISTER_ACTION_DECLARATION(af_type::aliens_active_action
    , alien_factory_aliens_active_action);

HPX_REGISTER_ACTION_DECLARATION(af_type::migrate_update_count_action
    , alien_factory_migrate_update_count_action);

HPX_REGISTER_ACTION_DECLARATION(af_type::antibodies_deficit_action
    , alien_factory_antibodies_deficit_action);

#endif //IMMUNE_SYSTEM_FOREIGN_BODIES_FACTORY_HPP