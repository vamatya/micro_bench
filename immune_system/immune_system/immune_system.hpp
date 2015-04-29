//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#if !defined(IMMUNE_SYSTEM_ANTIBODIES_HPP)
#define IMMUNE_SYSTEM_ANTIBODIES_HPP

#include <hpx/hpx.hpp>
#include <hpx/hpx_fwd.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/util/tuple.hpp>

#include <immune_system/immune_system/server/antibodies_factory.hpp>

inline std::pair<std::size_t, std::vector<hpx::id_type> > 
distribute_ab_factory(std::vector<hpx::id_type>& localities
, hpx::components::component_type type);

HPX_PLAIN_ACTION(distribute_ab_factory);

inline std::pair<std::size_t, std::vector<hpx::id_type> >
distribute_ab_factory(std::vector<hpx::id_type>& localities
, hpx::components::component_type type)
{
    typedef std::pair<std::size_t, std::vector<hpx::id_type> > result_type;
    result_type res;

    if (localities.size() == 0) return res;

    hpx::id_type this_loc = localities[0];

    std::size_t worker_threads = hpx::get_os_thread_count();

    // Factory per locality
    std::size_t num_ab_factory = 1; 

    hpx::future<std::vector<hpx::id_type> > fut_vec
        = hpx::new_<immune_system::server::antibodies_factory[]>(this_loc
            , num_ab_factory);

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
            hpx::id_type id = locs_first[0];
            hpx::future<result_type> res_fut
                = hpx::async<distribute_ab_factory_action>(id, locs_first, type);

            ab_factory_futures.emplace(ab_factory_futures.end()
                , std::move(res_fut));
        }

        if (locs_second.size() > 0)
        {
            hpx::id_type id = locs_second[0];
            hpx::future<result_type> res_fut
                = hpx::async<distribute_ab_factory_action>(id, locs_second, type);
            
            ab_factory_futures.emplace(ab_factory_futures.end()
                , std::move(res_fut));
        }
    }

    res.first = num_ab_factory;
    res.second = fut_vec.get();

    typedef hpx::future<result_type> fut_result_type;

    while (!ab_factory_futures.empty())
    {
        hpx::wait_any(ab_factory_futures);

        std::size_t ct = 0;
        std::vector<std::size_t> pos;

        for (fut_result_type& f: ab_factory_futures)
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

    hpx::id_type id = localities[0];

    hpx::future < std::pair<std::size_t, std::vector<hpx::id_type> > > async_result
        = hpx::async<distribute_ab_factory_action>(id, localities, type);


    std::pair < std::size_t, std::vector<hpx::id_type> > result = async_result.get();
    id_vector_type ab_factories = result.second;
    std::vector<hpx::future<void> > init_futures;

    std::size_t num_ab_factories = result.first;
    init_futures.reserve(num_ab_factories);


    std::size_t rank = 0;
    std::vector<hpx::future<void> > resolve_names_fut, spawn_future;
    resolve_names_fut.reserve(num_ab_factories);
    spawn_future.reserve(num_ab_factories);

    for (hpx::id_type& id : ab_factories)
    {
//         init_futures.push_back(
//             hpx::async<typename AntiBodyFactory::init_abf_action>(
//                 id, alien_factories, id, num_loc, max_aliens));
        resolve_names_fut.push_back(
            hpx::async<typename AntiBodyFactory::init_resolve_names_action>(
            id, alien_factories, id, num_loc, max_aliens_perloc
            , ab_factories, rank));
        ++rank;
    }
    //hpx::wait_all(init_futures);
    hpx::wait_all(resolve_names_fut);

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
    typedef hpx::util::tuple<bool, hpx::id_type> tup_type;

    typedef hpx::util::tuple<hpx::future<bool>, hpx::id_type> tup_fut_type;

    std::vector<tup_type> vec_aliens_active;
    std::vector<tup_fut_type> vec_fut_aliens_active;

    hpx::id_type invalid_type;

    std::size_t active_alien_factories_count = aliens_factories.size();

    typedef ForeignBodiesFactory::create_aliens_action create_aliens_action_type;

    while(alien_factories_active)
    {
        for (hpx::id_type id : aliens_factories)
        {
            hpx::future<void> fut_temp = hpx::async<create_aliens_action_type>(id);
            fut_temp.get();
            hpx::future<bool> f = hpx::async<active_action_type>(id);
            vec_fut_aliens_active.emplace(
                vec_fut_aliens_active.end()
                , hpx::util::forward_as_tuple(std::move(f), id));
        }

        //num_antibodies_to_create: for now just create the number 
        //      reported by the first antibody factory

        typedef AntiBodiesFactory::target_aliens_action target_action;

        std::vector<hpx::future<void> > ret_fut;
        
        //TODO: target just active alien factory
        for (tup_fut_type& tft: vec_fut_aliens_active)
        {
            // Aliens Active
            if (hpx::util::get<0>(tft).get())
            {
                for (hpx::id_type id: ab_factories)
                {
                    ret_fut.push_back(hpx::async<target_action>(
                        id, hpx::util::get<1>(tft)));
                }
            }
            else
            {
                --active_alien_factories_count;
            }
        }

        hpx::wait_all(ret_fut);

        if (!active_alien_factories_count)
        {
            alien_factories_active = false;
        }

        vec_fut_aliens_active.clear();
    }
}

//HPX_PLAIN_ACTION(process_foreignbodies);


#endif // IMMUNE_SYSTEM_ANTIBODIES_HPP


// What is the total max_aliens?
// create_num antibodies not exceeding the limit for the locality. 
// target_aliens (has to be successful, if not, destroy object through 
// out of context..
// aliens_active??