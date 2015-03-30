
//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/hpx_fwd.hpp>

//#include <immune_system/immune_system/antibodies.hpp>
#include <immune_system/immune_system/immune_system.hpp>
#include <immune_system/immune_system/foreign_bodies.hpp>



int hpx_main(boost::program_options::variables_map & vm)
{

    std::vector<hpx::id_type> alien_fac =
        create_alien_factory<immune_system::server::alien_factory>(vm);

    std::vector<hpx::id_type> antibody_fac =
        create_ab_factory<immune_system::server::antibodies_factory>(vm, alien_fac);

    

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    boost::program_options::options_description desc(
        "Usage: " HPX_APPLICATION_STRING " [options]");// = params_desc();

    desc.add_options()
        (
            "max-aliens-num"
          , boost::program_options::value<std::size_t>()->default_value(10000)
          , "Max Number of Aliens to be created per locality"
        )
        (
            "ab-num"
          , boost::program_options::value<std::size_t>()->default_value(1)
          , "Number of Antibodies to be created for each Alien"	
        )
        (
            "total-alien-factory"
          , boost::program_options::value<std::size_t>()->default_value(1)
          , "Total number of alien factory in a given run (with one locality per node)"
        )
        ;

    return hpx::init(desc, argc, argv);
}