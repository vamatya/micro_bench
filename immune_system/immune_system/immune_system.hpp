//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#if !defined(IMMUNE_SYSTEM_ANTIBODIES_HPP)
#define IMMUNE_SYSTEM_ANTIBODIES_HPP

#include <hpx/hpx.hpp>
#include <hpx/hpx_fwd.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/components/distributing_factory/distributing_factory.hpp>
#include <hpx/util/tuple.hpp>

#include <immune_system/immune_system/server/antibodies_factory.hpp>

//typedef hpx::util::tuple<bool

inline std::pair<std::size_t, std::vector<hpx::util::remote_locality_result> >
distribute_ab_factory(std::vector<hpx::id_type>& localities
, hpx::components::component_type type);

HPX_PLAIN_ACTION(distribute_ab_factory);

inline std::pair<std::size_t, std::vector<hpx::util::remote_locality_result> >
distribute_ab_factory(std::vector<hpx::id_type>& localities
, hpx::components::component_type type)
{
    typedef hpx::util::remote_locality_result value_type;
    typedef std::pair<std::size_t, std::vector<value_type> > result_type;

    result_type res;

    if (localities.size() == 0) return res;

    hpx::id_type this_loc = localities[0];

    typedef
        hpx::components::server::runtime_support::bulk_create_components_action
        action_type;

    std::size_t worker_threads = hpx::get_os_thread_count();

    // One factory per locality, curr impl
    std::size_t num_ab_factory = 1; //default 1 ab-factory per locality

    typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;

    future_type f;
    {
        hpx::lcos::packaged_action<action_type
            , std::vector<hpx::naming::gid_type> > p;
        p.apply(hpx::launch::async, this_loc, type, num_ab_factory);
        f = p.get_future();
    }

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
            hpx::lcos::packaged_action<distribute_ab_factory_action, result_type> p;
            hpx::id_type id = locs_first[0];
            p.apply(hpx::launch::async, id, boost::move(locs_first), type);
            ab_factory_futures.push_back(p.get_future());
        }

        if (locs_second.size() > 0)
        {
            hpx::lcos::packaged_action<distribute_ab_factory_action, result_type> p;
            hpx::id_type id = locs_second[0];
            p.apply(hpx::launch::async, id, boost::move(locs_second), type);
            ab_factory_futures.push_back(p.get_future());
        }
    }

    res.first = num_ab_factory;
    res.second.push_back(
        value_type(this_loc.get_gid(), type)
        );
    res.second.back().gids_ = boost::move(f.get());

    while (!ab_factory_futures.empty())
    {
        hpx::wait_any(ab_factory_futures);

        std::size_t ct = 0;
        std::vector<std::size_t> pos;

        BOOST_FOREACH(hpx::lcos::future<result_type> &f, ab_factory_futures)
        {
            if (f.is_ready())
            {
                pos.push_back(ct);
            }
            ++ct;
        }

        BOOST_FOREACH(std::size_t i, pos)
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
    boost::program_options::variables_map & vm
    , std::vector<hpx::id_type>& alien_factories)
{
    std::size_t num_antibodies = vm["ab-num"].as<std::size_t>();
    std::size_t max_aliens_perloc = vm["max-aliens-num"].as<std::size_t>();

    //TO DO : increase num ab factory per locality?
    
    typedef std::vector<hpx::id_type> id_vector_type;

    hpx::components::component_type type =
        hpx::components::get_component_type<AntiBodyFactory>();

    id_vector_type localities = hpx::find_all_localities(type);
    std::size_t num_loc = localities.size();

    using hpx::components::distributing_factory;

    hpx::id_type id = localities[0];
    hpx::future<std::pair<std::size_t
        , std::vector<hpx::util::remote_locality_result> > > async_result
        = hpx::async<distribute_ab_factory_action>(id, localities, type);

    id_vector_type ab_factories;
    std::vector<hpx::future<void> > init_futures;

    std::pair<std::size_t, std::vector<hpx::util::remote_locality_result> >
        result(boost::move(async_result.get()));

    std::size_t num_ab_factories = result.first;//();
    ab_factories.reserve(num_ab_factories);
    init_futures.reserve(num_ab_factories);


    std::vector<hpx::util::locality_result> res;
    res.reserve(result.second.size());
    BOOST_FOREACH(hpx::util::remote_locality_result const & rl, result.second)
    {
        res.push_back(rl);
    }

    BOOST_FOREACH(hpx::id_type id, hpx::util::locality_results(res))
    {
//         init_futures.push_back(
//         hpx::async<typename AntiBodyFactory::init_abf_action>(
//             id, alien_factories, id, num_loc, max_aliens));
        ab_factories.push_back(id);
    }
//     hpx::wait_all(init_futures);

    std::vector<hpx::future<void> > resolve_names_fut, spawn_future;
    resolve_names_fut.reserve(num_ab_factories);
    spawn_future.reserve(num_ab_factories);

    std::size_t rank = 0;
    BOOST_FOREACH(hpx::id_type const& id, ab_factories)
    {
        resolve_names_fut.push_back(
            hpx::async<typename AntiBodyFactory::init_resolve_names_action>(
                id, alien_factories, id, num_loc, max_aliens_perloc
                , ab_factories, rank));
        ++rank;
//         spawn_future.push_back(hpx::async<spawn_action>(
//             id, num_antibodies));
//         spawn_future.push_back(hpx::async <
//             typename AntiBodyFactory::spawn_n_antibodies_action<
//             immune_system::server::antibodies> >
//             (id, num_antibodies));
    }

    hpx::wait_all(resolve_names_fut);
    hpx::wait_all(spawn_future);

    return ab_factories;
}

template <typename AntiBodiesFactory, typename ForeignBodiesFactory>
void process_foreignbodies(boost::program_options::variables_map & vm
    , std::vector<hpx::id_type> & ab_factories
    , std::vector<hpx::id_type> & aliens_factories)
{
    bool alien_factories_active = true;

    //std::size_t num_antibodies_rate = vm["max-aliens-num"].as<std::size_t>();// / ;

    std::size_t max_aliens_per_loc = vm["max-aliens-num"].as<std::size_t>();
    // aliens_active??
    typedef ForeignBodiesFactory::aliens_active_action active_action_type;
    hpx::util::tuple<bool, hpx::id_type> tup_type;
    hpx::util::tuple<hpx::future<bool>, hpx::id_type> tup_fut_type;

    std::vector<tup_type> vec_aliens_active;
    //std::vector<hpx::future<bool> > aliens_active_future;
    std::vector<tup_fut_type> vec_fut_aliens_active;

    hpx::id_type invalid_type;

    std::size_t active_alien_factories_count = aliens_factories.size();

    while (alien_factories_active)
    {
        BOOST_FOREACH(hpx::id_type id, aliens_factories)
        {
            //aliens_active_future.push_back(hpx::async<active_action_type>);
            tup_fut_type temp;
            hpx::util::get<0>(temp) = hpx::async<active_action_type>(id);
            hpx::util::get<1>(temp) = id;
            vec_fut_aliens_active.push_back(temp);
        }

//         //num_antibodies_to_create: for now just create the number 
//         //      reported by the first antibody factory

        typedef AntiBodyFactory::target_alien_action target_action;

        std::size_t active_alien_factories_count = 
        
        BOOST_FOREACH(tup_fut_type tft, vec_fut_aliens_active)
        {
            if (!hpx::util::get<0>(tft).get())
            {
                BOOST_FOREACH(hpx::id_type id, ab_factories)
                {
                    hpx::async<target_action>(id, hpx::util::get<1>);
                }
            }
            else
            {
                --active_alien_factories_count;
            }
        }

        if (!active_alien_factories_count)
        {
            alien_factories_active = false;
        }        
    }
}


#endif // IMMUNE_SYSTEM_ANTIBODIES_HPP


// What is the total max_aliens?
// create_num antibodies not exceeding the limit for the locality. 
// target_aliens (has to be successful, if not, destroy object through 
// out of context..
// aliens_active??