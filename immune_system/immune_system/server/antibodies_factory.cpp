
//  Copyright (c) 2015 Vinay C Amatya
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// #include <hpx/hpx_fwd.hpp>
// #include <hpx/hpx.hpp>
// #include <hpx/include/components.hpp>
//#include <hpx/runtime/components/server/managed_component_base.hpp>

#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/components/distributing_factory/distributing_factory.hpp>


#include "antibodies.hpp"
#include "antibodies_factory.hpp"
//#include "foreign_bodies_factory.hpp"
//#include "foreign_bodies.hpp"




//###########################################################################

void components::antibodies_factory::init_abf()
{

}

void components::antibodies_factory::resolve_names(std::vector<hpx::id_type> ids)
{
    fac_ids_ = ids;
}

bool components::antibodies_factory::alien_factory_active()
{
    return true;
}

bool components::antibodies_factory::get_alien_factory(hpx::id_type al_factory)
{
    al_factory_ = al_factory;
    return true;
}

bool components::antibodies_factory::scan_aliens()
{
    return true;
}


void components::antibodies_factory::spawn_antibody()
{
    typedef ::components::antibodies::alien_connect_action;
}



void components::antibodies_factory::spawn_antibodies(std::size_t num)
{
    typedef hpx::util::remote_locality_result value_type;
    typedef std::pair<std::size_t, std::vector<value_type> > result_type;

    result_type res;


    typedef std::vector<hpx::id_type> id_vector_type;

    hpx::components::component_type c_type =
        hpx::components::get_component_type<::components::antibodies>();

    hpx::id_type this_loc = hpx::find_here();

    typedef
        hpx::components::server::runtime_support::bulk_create_components_action
        action_type;

    typedef hpx::future<std::vector<hpx::naming::gid_type> > future_type;

    future_type f;
    {
        hpx::lcos::packaged_action < action_type
            , std::vector<hpx::naming::gid_type> > p;
        p.apply(hpx::launch::async, this_loc, c_type, num);
        f = p.get_future();
    }


    res.first = num;
    res.second.back().gids_ = boost::move(f.get());

    antibodies_.reserve(num);

    std::vector<hpx::util::locality_result> res2;
    BOOST_FOREACH(hpx::util::remote_locality_result const& r1, res.second)
    {
        res2.push_back(r1);
    }

    BOOST_FOREACH(hpx::id_type id, hpx::util::locality_results(res2))
    {
        antibodies_.push_back(id);
    }
}


void components::antibodies_factory::kill_antibodies()
{

}


//###########################################################################

//typedef ::components::antibodies_factory ab_factory_type;
typedef ::components::antibodies_factory ab_factory_type;

//HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
//    hpx::components::managed_component<ab_factory_type>, ab_ab_factory_type);
    

HPX_REGISTER_ACTION(ab_factory_type::init_abf_action
   , antibodies_factory_init_abf_action);

HPX_REGISTER_ACTION(ab_factory_type::spawn_antibody_action
   , antibodies_factory_spawn_antibody_action);

HPX_REGISTER_ACTION(
   ::components::antibodies_factory::spawn_antibodies_action,
   antibodies_factory_spawn_antibodies_action
 //   );

HPX_REGISTER_ACTION(ab_factory_type::alien_factory_active_action
   , antibodies_factory_alien_factory_active_action);


/////////////////////////////////////////////////////////////////////////////

// HPX_REGISTER_COMPONENT_MODULE();
// 
// HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
// hpx::components::managed_component< ::components::antibodies>
// , antibodies_component);
/*typedef ::components::alien_factory alien_factory_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
    hpx::components::managed_component<alien_factory_type>, alien_alien_factory_type);


HPX_REGISTER_ACTION(alien_factory_type::spawn_action
    , alien_factory_spawn_action);

HPX_REGISTER_ACTION(alien_factory_type::spawn_n_aliens_action
    , alien_factory_spawn_n_aliens_action);


//////////////////////////////////////////////////////////////////////////////

HPX_REGISTER_ACTION(
    ::components::aliens::ab_connect_action,
    aliens_ab_connect_action
    );

HPX_REGISTER_ACTION(
    ::components::aliens::alien_active_action,
    aliens_alien_active_action
    );

HPX_REGISTER_ACTION(
    ::components::aliens::send_spawn_signal_action,
    aliens_send_spawn_signal_action
    );
*/

