



#include "foreign_bodies.hpp"


HPX_REGISTER_ACTION(
    ::components::aliens::ab_connect_action,
    aliens_ab_connect_action
    );

HPX_REGISTER_ACTION_DECLARATION(
    ::components::aliens::alien_active_action,
    aliens_alien_active_action
    );

HPX_REGISTER_ACTION_DECLARATION(
    ::components::aliens::send_spawn_signal_action,
    aliens_send_spawn_signal_action