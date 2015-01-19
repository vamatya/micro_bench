# Copyright (c) 2007-2012 Hartmut Kaiser
# Copyright (c) 2015      Vinay C Amatya
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

###############################################################################

set(server_sources
    server/antibodies_factory.cpp
    #server/foreign_bodies_factory.cpp
    #server/foreign_bodies.cpp
    server/antibodies.cpp
    )

set(server_headers
    server/antibodies_factory.hpp
    server/antibodies.hpp
    )

set(source_files
    immune_system.cpp
    )

set(header_files
    immune_system.hpp
   ) 

###############################################################################

set(headers
    ${server_headers}
    ${header_files}
    )

set(sources
    ${server_sources}
    ${source_files}
    )

###############################################################################
source_group("Source Files\\Server" FILES ${server_sources})
source_group("Header Files\\Server" FILES ${server_headers})
source_group("Header Files" FILES ${header_files})
source_group("Source Files" FILES ${source_files})

###############################################################################
add_hpx_component(immune_system
  SOURCES ${sources}
  HEADERS ${headers}
  DEPENDENCIES distributing_factory_component
  FOLDER "Micro_bench/Immune_system"
  )






#################################################################################  
#set(components
#    #antibodies
#    antibodies_factory
#    )

#foreach(component ${components})
    
#   add_hpx_component(${component}
#        AUTOGLOB
#        HEADER_GLOB "${component}.h*"
#        SOURCE_GLOB "${component}.c*"
#        FOLDER "Micro_bench/Immune_system/Immune_system/${component}")
#endforeach()
