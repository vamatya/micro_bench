//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/hpx_fwd.hpp>

#include <immune_system/immune_system_simple/server/antibodies_factory.hpp>
#include <immune_system/immune_system_simple/immune_system.hpp>



int hpx_main(boost::program_options::variables_map & vm)
{
    std::vector<hpx::id_type> ab_fac =
        create_ab_factory<immune_system::server::antibodies_factory>(vm);

    typedef immune_system::server::antibodies_factory::print_stat_action action_type;

    std::vector<hpx::future<void> > stats_fut;
    BOOST_FOREACH(hpx::id_type id, ab_fac)
    {
        stats_fut.push_back(hpx::async<action_type>(id));
    }

    hpx::wait_all(stats_fut);

    BOOST_FOREACH(hpx::future<void>& fut, stats_fut)
    {
        fut.get();
    }

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    boost::program_options::options_description desc(
        "Usage: " HPX_APPLICATION_STRING " [options]");

    desc.add_options()
        (
            "ab-num"
          , boost::program_options::value<std::size_t>()->default_value(100)
          , "Number of Antibodies to be created for each Alien"	
        )
        ;

    return hpx::init(desc, argc, argv);
}