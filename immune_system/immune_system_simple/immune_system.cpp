
#include <hpx/hpx.hpp>
#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>

#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/util/portable_binary_iarchive.hpp>
#include <hpx/util/portable_binary_oarchive.hpp>

#include <hpx/components/distributing_factory/distributing_factory.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/export.hpp>

#include <immune_system/immune_system_simple/server/antibodies_factory.hpp>


HPX_REGISTER_COMPONENT_MODULE();

