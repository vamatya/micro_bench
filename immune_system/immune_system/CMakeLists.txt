# Copyright (c) 2007-2012 Hartmut Kaiser
# Copyright (c) 2015      Vinay C Amatya
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

###############################################################################

set(abf_server_sources
    server/antibodies_factory.cpp
    #server/foreign_bodies_factory.cpp
    #server/foreign_bodies.cpp
    #server/antibodies.cpp
    )

set(abf_server_headers
    server/antibodies_factory.hpp
    )

set(abf_sources
    antibodies_factory.cpp
    )

    #*************************************

set(ab_server_sources
    server/antibodies.cpp
    )

set(ab_server_headers
    server/antibodies.hpp
    )

set(ab_sources
    antibodies.cpp
    )

###############################################################################
set(server_sources
    ${abf_server_sources}
    ${ab_server_sources}
    )

set(server_headers
    ${abf_server_headers}
    $ab_server_headers}
    )

#set(headers
    #${client_headers}
    #${stubs_headers}
    #${server_headers}
    #${header_files})

set(sources
    ${abf_sources}
    ${ab_sources}
    #${server_sources}
    #${source_files}
    )

###############################################################################
source_group("Source Files\\Server" FILES ${server_sources})
source_group("Header Files\\Server" FILES ${server_headers})
#source_group("Header Files" FILES ${header_files})
source_group("Source Files" FILES ${sources})

###############################################################################
#add_hpx_component(immune_system
#  SOURCES ${sources}
#  HEADERS ${headers}
#  DEPENDENCIES distributing_factory_component
#  FOLDER "Micro_bench/Immune_system/Immune_system")


#################################################################################  

set(ab_comp_sources
    ${ab_server_sources}
    ${ab_sources}
    )

set(ab_comp_headers
    ${ab_server_headers}
    )

add_hpx_component(antibodies
  SOURCES ${ab_comp_sources}
  HEADERS ${ab_comp_headers}
  DEPENDENCIES distributing_factory_component
  FOLDER "Micro_bench/Immune_system/Ab_component")


###############################################################################

set(abf_comp_sources
    ${abf_server_sources}
    ${abf_sources}
    )

set(abf_comp_headers
    ${abf_server_headers}
    )

add_hpx_component(antibodies_factory
  SOURCES ${abf_comp_sources}
  HEADERS ${abf_comp_headers}
  DEPENDENCIES distributing_factory_component antibodies_component
  FOLDER "Micro_bench/Immune_system/Abf_component")
