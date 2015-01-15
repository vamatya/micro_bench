
#include "./antibodies.hpp"

HPX_REGISTER_COMPONENT_MODULE();

/*HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
hpx::components::managed_component< ::components::alien_factory>
, alien_factory_component);

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
hpx::components::managed_component< ::components::aliens>
, aliens_component);*/

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
hpx::components::managed_component< ::components::antibodies_factory>
, antibodies_factory_component);

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(
hpx::components::managed_component< ::components::antibodies>
, antibodies_component);
