//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(IMMUNE_SYSTEM_ALIENS_HPP)
#define IMMUNE_SYSTEM_ALIENS_HPP

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/components.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include<immune_system/immune_system/server/foreign_bodies_factory.hpp>


void alien_location_ranks(std::vector<std::size_t>& alien_loc_ranks
    , std::size_t const& num_alien_factories, std::size_t const& num_locs)
{
    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<std::size_t> dist(0, num_locs - 1);

    alien_loc_ranks.push_back(dist(gen));

    while (alien_loc_ranks.size() != num_alien_factories)
    {
        std::size_t temp = dist(gen);
        for (std::size_t i : alien_loc_ranks)
        {
            if (temp == i)
                temp = dist(gen);
            else
                break;
        }
        alien_loc_ranks.push_back(temp);
    }
}

inline std::pair<std::size_t, std::vector<hpx::id_type> >
distribute_alien_factory(std::vector<hpx::id_type>& localities
, hpx::components::component_type type);

HPX_PLAIN_ACTION(distribute_alien_factory);

inline std::pair<std::size_t, std::vector<hpx::id_type> >
distribute_alien_factory(std::vector<hpx::id_type>& localities
, hpx::components::component_type type)
{
    typedef std::pair<std::size_t, std::vector<hpx::id_type> > result_type;

    result_type res;

    if (localities.size() == 0) return res;

    hpx::id_type this_loc = localities[0];

    std::size_t worker_threads = hpx::get_os_thread_count();
    // Alien Factories per locality
    std::size_t num_alien_factory = 1;

    hpx::future<std::vector<hpx::id_type> > fut_vec
        = hpx::new_<immune_system::server::antibodies_factory[]>(
            this_loc, num_alien_factory);

    std::vector<hpx::future<result_type> > alien_factory_futures;
    alien_factory_futures.reserve(2);

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
                = hpx::async<distribute_alien_factory_action>(id, locs_first, type);

            alien_factory_futures.emplace(alien_factory_futures.end()
                , std::move(res_fut));
        }

        if (locs_second.size() > 0)
        {
            hpx::id_type id = locs_second[0];
            hpx::future<result_type> res_fut
                = hpx::async<distribute_alien_factory_action>(
                    id, locs_second, type);

            alien_factory_futures.emplace(alien_factory_futures.end()
                , std::move(res_fut));
        }
    }



    res.first = num_alien_factory;
    res.second = fut_vec.get();
    
    typedef hpx::future<result_type> fut_result_type;

    while (!alien_factory_futures.empty())
    {
        hpx::wait_any(alien_factory_futures);
        std::size_t ct = 0;
        std::vector<std::size_t> pos;

        for (fut_result_type& f: alien_factory_futures)
        {
            if (f.is_ready())
            {
                pos.push_back(ct);
            }
            ++ct;
        }
        
        for (std::size_t i: pos)
        {
            result_type r = alien_factory_futures.at(i).get();
            res.second.insert(res.second.end(), r.second.begin(), r.second.end());
            res.first += r.first;
            alien_factory_futures.erase(alien_factory_futures.begin() + i);
        }
    }
    return res;
}


template <typename AlienFactory>
inline std::vector<hpx::id_type> create_alien_factory(
    boost::program_options::variables_map & vm)
{
    typedef std::vector<hpx::id_type> id_vector_type;

    std::size_t num_alien_factories = vm["total-alien-factory"].as<std::size_t>();
    std::size_t max_aliens_limit = vm["max-aliens-num"].as<std::size_t>();

    hpx::components::component_type type =
        hpx::components::get_component_type<AlienFactory>();

    id_vector_type localities = hpx::find_all_localities(type);
    std::size_t num_loc = localities.size();

    BOOST_ASSERT(num_loc >= num_alien_factories); // Throw HPX error, here.
    if (num_loc < num_alien_factories)
        num_alien_factories = 1;            // default to 1 locality. 
        
    std::vector<std::size_t> alien_loc_ranks;
    alien_location_ranks(alien_loc_ranks, num_alien_factories, num_loc);

    BOOST_ASSERT(num_alien_factories = alien_loc_ranks.size());

    
    //alien_factory localities
    std::vector<hpx::id_type> alien_fac_locs;

    for(std::size_t i : alien_loc_ranks)
    {
        alien_fac_locs.push_back(localities[i]);
    }

    hpx::id_type id = alien_fac_locs[0]; //localities[0];
    hpx::future<std::pair<std::size_t, std::vector<hpx::id_type> > > async_result
        = hpx::async<distribute_alien_factory_action>(id, alien_fac_locs, type);

    std::pair<std::size_t, std::vector<hpx::id_type> > result
        = async_result.get();

    id_vector_type alien_factories = result.second;
    
    std::vector<hpx::future<void> > init_futures;

    BOOST_ASSERT(num_alien_factories == result.first);
    alien_factories.reserve(num_alien_factories);
    init_futures.reserve(num_alien_factories);


    for (hpx::id_type& id: alien_factories)
    {
        init_futures.push_back(
            hpx::async<typename AlienFactory::init_action>(id,id,max_aliens_limit));
    }

    hpx::wait_all(init_futures);

    std::vector<hpx::future<void> > resolve_names_fut;
    resolve_names_fut.reserve(num_alien_factories);
    
    return alien_factories;
}

template <typename AlienFactory>
void activate_aliens(std::vector<hpx::id_type>& al_factories)
{
    typedef AlienFactory::create_aliens_action create_action_type;

    BOOST_FOREACH(hpx::id_type id, al_factories)
    {
        hpx::async<create_action_type>(id);
    }
}

//HPX_PLAIN_ACTION(activate_aliens);
#endif //IMMUNE_SYSTEM_ALIENS_HPP