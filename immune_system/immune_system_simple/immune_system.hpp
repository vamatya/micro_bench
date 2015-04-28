//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef IMMUNE_SYSTEM_SIMPLE_HPP
#define IMMUNE_SYSTEM_SIMPLE_HPP

#include <hpx/hpx.hpp>
#include <hpx/hpx_fwd.hpp>
#include <hpx/include/iostreams.hpp>
//#include <hpx/components/distributing_factory/distributing_factory.hpp>

#include <immune_system/immune_system_simple/server/antibodies_factory.hpp>

//inline std::pair<std::size_t, std::vector<hpx::util::remote_locality_result> >
inline std::pair<std::size_t, std::vector<hpx::id_type> >
distribute_ab_factory(std::vector<hpx::id_type> localities
, hpx::components::component_type type);

HPX_PLAIN_ACTION(distribute_ab_factory);

//inline std::pair<std::size_t, std::vector<hpx::util::remote_locality_result> >

inline std::pair<std::size_t, std::vector<hpx::id_type> >
distribute_ab_factory(std::vector<hpx::id_type> localities
, hpx::components::component_type type)
{
//     typedef hpx::util::remote_locality_result value_type;
//     typedef std::pair<std::size_t, std::vector<value_type> > result_type;
    typedef std::pair <std::size_t, std::vector<hpx::id_type> > result_type;
   result_type res;

    //if (localities.size() == 0) return res;

    hpx::id_type this_loc = localities[0];

//     typedef
//         hpx::components::server::runtime_support::bulk_create_components_action
//         action_type;

    std::size_t worker_threads = hpx::get_os_thread_count();

    std::size_t num_ab_factory = worker_threads;

/*    typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;*/

//     future_type f;
//     {
//         hpx::lcos::packaged_action<action_type
//             , std::vector<hpx::naming::gid_type> > p;
//         p.apply(hpx::launch::async, this_loc, type, num_ab_factory);
//         f = p.get_future();
//     }

//     hpx::future<std::vector<hpx::id_type> > fut_vec 
//         = hpx::new_<
    hpx::future<std::vector<hpx::id_type> > fut_vec
        = hpx::new_<immune_system::server::antibodies_factory[]>(
            this_loc, num_ab_factory);

    std::vector<hpx::future<result_type> > ab_factory_futures;
    ab_factory_futures.reserve(2);

    if (localities.size() > 1)
    {
        std::size_t half = (localities.size() / 2) + 1;
        std::vector<hpx::id_type> locs_first(localities.begin() + 1
            , localities.begin() + half);
        std::vector<hpx::id_type> locs_second(localities.begin() + half
            , localities.end());

        if (locs_first.size() > 0)
        {
//             hpx::lcos::packaged_action<distribute_ab_factory_action, result_type> p;
            hpx::id_type id = locs_first[0];
//             p.apply(hpx::launch::async, id, boost::move(locs_first), type);
            hpx::future<result_type> res_fut
                = hpx::async<distribute_ab_factory_action>(id, std::move(
                    locs_first), type);

            ab_factory_futures.emplace(
                ab_factory_futures.end(), std::move(res_fut));

            //ab_factory_futures.push_back(res_fut);
            //ab_factory_futures.push_back(p.get_future());
        }

        if (locs_second.size() > 0)
        {
//             hpx::lcos::packaged_action<distribute_ab_factory_action, result_type> p;
             hpx::id_type id = locs_second[0];
//             p.apply(hpx::launch::async, id, boost::move(locs_second), type);
            hpx::future<result_type> res_fut
                = hpx::async<distribute_ab_factory_action>(id, std::move(
                    locs_second), type);

            ab_factory_futures.emplace(
                ab_factory_futures.end(), std::move(res_fut));

            //ab_factory_futures.push_back(res_fut);
            //ab_factory_futures.push_back(p.get_future());
        }
    }
    
    res.first = num_ab_factory;
    res.second = fut_vec.get();
//     res.second.push_back(
//         value_type(this_loc.get_gid(), type)
//         );
//     res.second.back().gids_ = boost::move(f.get());



    while (!ab_factory_futures.empty())
    {
        //hpx::wait_any(ab_factory_futures);
        hpx::wait_any(ab_factory_futures);

        std::size_t ct = 0;
        std::vector<std::size_t> pos;

        typedef hpx::future<result_type> fut_result_type;

        for (fut_result_type& f : ab_factory_futures)
        {
            if (f.is_ready())
            {
                pos.push_back(ct);
            }
            ++ct;
        }

        for (std::size_t i: pos)
        {
            result_type r = ab_factory_futures.at(i).get();
            res.second.insert(res.second.end(), r.second.begin(), r.second.end());
            res.first += r.first;
            ab_factory_futures.erase(ab_factory_futures.begin() + i);
        }
    }

    return res;
}

template <typename AntiBodyFactory>
inline std::vector<hpx::id_type> create_ab_factory(
    boost::program_options::variables_map & vm)
{
    std::size_t num_antibodies = vm["ab-num"].as<std::size_t>();


    typedef std::vector<hpx::id_type> id_vector_type;

    hpx::components::component_type type =
        hpx::components::get_component_type<AntiBodyFactory>();

    id_vector_type localities = hpx::find_all_localities(type);
    std::size_t num_loc = localities.size();

    //using hpx::components::distributing_factory;

    hpx::id_type id = localities[0];
//     hpx::future<std::pair<std::size_t
//         , std::vector<hpx::util::remote_locality_result> > > async_result
//         = hpx::async<distribute_ab_factory_action>(id, localities, type);

    hpx::future<std::pair<std::size_t, std::vector<hpx::id_type> > > async_result
        = hpx::async<distribute_ab_factory_action>(id, localities, type);

    id_vector_type ab_factories;
    std::vector<hpx::future<void> > init_futures;

//     std::pair<std::size_t, std::vector<hpx::util::remote_locality_result> >
//         result(boost::move(async_result.get()));
    std::pair<std::size_t, std::vector<hpx::id_type> > result = async_result.get();

    std::size_t num_ab_factories = result.first;//();
    ab_factories.reserve(num_ab_factories);
    init_futures.reserve(num_ab_factories);


//     std::vector<hpx::util::locality_result> res;
//     res.reserve(result.second.size());
//     BOOST_FOREACH(hpx::util::remote_locality_result const & rl, result.second)
//     {
//         res.push_back(rl);
//     }
    id_vector_type res = result.second;

    for (hpx::id_type& id : res)
    {
        ab_factories.push_back(id);
    }

    hpx::wait_all(init_futures);

    std::vector<hpx::future<void> > resolve_names_fut, spawn_future;
    resolve_names_fut.reserve(num_ab_factories);
    spawn_future.reserve(num_ab_factories);
    
    
    typedef immune_system::server::antibodies_factory::spawn_antibodies_action 
        spawn_action;

    std::size_t rank = 0;

    for (hpx::id_type const& id: ab_factories)
    {
        resolve_names_fut.push_back(
            hpx::async<typename AntiBodyFactory::resolve_names_action>(
                id,ab_factories, rank));
        ++rank;
        spawn_future.push_back(hpx::async<spawn_action>(
            id, num_antibodies));
    }

    hpx::wait_all(resolve_names_fut);
    hpx::wait_all(spawn_future);

    return ab_factories;
}

#endif // IMMUNE_SYSTEM_SIMPLE_HPP


