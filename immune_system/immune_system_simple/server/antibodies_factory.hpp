//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef IMMUNE_SYSTEM_SIMPLE_SERVER_ANTIBODIES_FACTORY_HPP
#define IMMUNE_SYSTEM_SIMPLE_SERVER_ANTIBODIES_FACTORY_HPP

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>
#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>

//#include <hpx/components/distributing_factory/distributing_factory.hpp>
//#include <hpx/runtime/components/new.hpp>

//#include <immune_system/server/foreign_bodies_factory.hpp>
#include "antibodies.hpp"


namespace immune_system{
    namespace server
    {
        // Foreign Bodies Detection and Generating Antibodies. 
        // Maintain Stat of objects created
        struct antibodies_factory
            : hpx::components::managed_component_base < antibodies_factory >
        {

            //Note: For now maintain list of all spawned antibodies. 

            antibodies_factory(){}

            antibodies_factory(hpx::id_type my_id)
                : my_id_(my_id)
            {}

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

                      
            //Spawn \N Antibodies
            void spawn_antibodies(std::size_t num)
            {
                //typedef hpx::util::remote_locality_result value_type;
               // typedef std::pair<std::size_t, std::vector<value_type> > result_type;

                //result_type res;


                typedef std::vector<hpx::id_type> id_vector_type;

//                 hpx::components::component_type c_type =
//                     hpx::components::get_component_type<immune_system::server::antibodies>();

                hpx::id_type this_loc = hpx::find_here();

//                 typedef
//                     hpx::components::server::runtime_support::bulk_create_components_action
//                     action_type;
// 
//                 typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;
// 
//                 future_type f;
//                 {
//                     hpx::lcos::packaged_action < action_type
//                         , std::vector<hpx::naming::gid_type> > p;
//                     p.apply(hpx::launch::async, this_loc, c_type, num);
//                     f = p.get_future();
//                 }
// 
//                 res.first = num;
//                 res.second.push_back(
//                     value_type(this_loc.get_gid(), c_type));
//                 res.second.back().gids_ = boost::move(f.get());
// 
//                 antibodies_.reserve(num);
// 
//                 std::vector<hpx::util::locality_result> res2;
//                 BOOST_FOREACH(hpx::util::remote_locality_result const& r1, res.second)
//                 {
//                     res2.push_back(r1);
//                 }

                hpx::future<std::vector<hpx::id_type> > fut =
                    hpx::new_<immune_system::server::antibodies[]>(this_loc, num);

                //hpx::wait_all(fut);

                std::vector<hpx::id_type> vec_comp = fut.get();

                //typedef typename hpx::id_type h_id_type;
                std::vector<hpx::id_type>::const_iterator const_itr_b 
                    = vec_comp.begin();

                for (std::vector<hpx::id_type>::const_iterator i = const_itr_b;
                    i != vec_comp.end(); ++i)
                {
                    antibodies_.push_back(*i);
                }

//                 BOOST_FOREACH(hpx::id_type& id, vec_comp)
//                 {
//                     antibodies_.push_back(id);
//                 }

                //BOOST_FOREACH(hpx::id_type id, hpx::util::locality_results(res2))

//                 BOOST_FOREACH(hpx::id_type id, vec_comp)
//                 {
//                     //antibodies_.push_back(id);
//                 }


            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, spawn_antibodies);
                                   
            void print_stat()
            {
                std::cout << "My Rank:" << my_rank_ << std::endl;
                std::cout << "Total Antibodies Created:" << antibodies_.size() << std::endl;
            }

            HPX_DEFINE_COMPONENT_ACTION(antibodies_factory, print_stat);
        private:
            hpx::id_type my_id_;
            hpx::id_type al_factory_;
            std::size_t produced_count;
            std::size_t my_rank_;

            std::vector<hpx::id_type> fac_ids_;
            std::vector<hpx::id_type> antibodies_;
        };
    }
}

 ////////////////////////////////////////////////////////

typedef immune_system::server::antibodies_factory abf_type;

HPX_REGISTER_ACTION_DECLARATION(abf_type::init_abf_action,
    abf_init_abf_action
    );

HPX_REGISTER_ACTION_DECLARATION(abf_type::spawn_antibodies_action,
    abf_spawn_antibodies_action
    );


HPX_REGISTER_ACTION_DECLARATION(abf_type::print_stat_action,
    abf_print_stat_action
    );

#endif //IMMUNE_SYSTEM_SIMPLE_ANTIBODIES_FACTORY_HPP
