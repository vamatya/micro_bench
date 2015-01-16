
//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>
#include <hpx/hpx_fwd.hpp>

#include <immune_system/immune_system/antibodies.hpp>
//#include <immune_system/immune_system/foreign_bodies.hpp>


//HPX_REGISTER_COMPONENT_MODULE();


// typedef ::components::antibodies_factory ab_factory_type;
// 
// HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
//     hpx::components::managed_component<ab_factory_type>, ab_ab_factory_type);

// typedef ::components::antibodies antibodies_type;
// 
// HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
//     hpx::components::managed_component<antibodies_type>, ab_antibodies_type);


int hpx_main(boost::program_options::variables_map & vm)
{
    std::vector<hpx::id_type> ab_fac =
        create_ab_factory<components::server::antibodies_factory>(vm);

    //std::vector<hpx::id_type> aln_fac =
        //create_alien_factory<components::alien_factory>(vm,ab_fac);

    return hpx::finalize();

}

int main(int argc, char* argv[])
{
    boost::program_options::options_description desc(
        "Usage: " HPX_APPLICATION_STRING " [options]");// = params_desc();

    desc.add_options()
        //(
        //    "aliens-num"
        //  , boost::program_options::value<std::size_t>()->default_value(1000)
        //  , "Number of Aliens to be created"
        //)
        (
            "ab-num"
          , boost::program_options::value<std::size_t>()->default_value(100)
          , "Number of Antibodies to be created for each Alien"	
        )
        ;

    return hpx::init(desc, argc, argv);
}