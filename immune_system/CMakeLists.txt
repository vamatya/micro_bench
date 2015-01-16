# Copyright (c) 2007-2012 Hartmut Kaiser
# Copyright (c) 2011      Bryce Lelbach
# Copyright (c) 2015      Vinay C Amatya
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# just recurse into all components subdirectories and execute the scripts there
set(subdirs
    immune_system)

# for all targets specified above
foreach(subdir ${subdirs})
  add_subdirectory(${subdir})

  set(sources
      ${subdir}_client.cpp)

  set(headers
      ${subdir}/${subdir}.hpp
      )

  source_group("Source Files" FILES ${sources})
  source_group("Header Files\\Immune_system" FILES ${headers})

  set(components
      #${subdir}_component
      antibodies_component
      antibodies_factory_component
      )

  # add example executable
  add_hpx_executable(${subdir}_client
    SOURCES ${sources}
    HEADERS ${headers}
    DEPENDENCIES ${components}
    FOLDER "Micro_bench/Immune_system")

  # add a custom target for this example
  add_hpx_pseudo_target(micro_bench.immune_system.${subdir})

  # make pseudo-targets depend on master pseudo-target
  add_hpx_pseudo_dependencies(micro_bench.immune_system
                              micro_bench.immune_system.${subdir})

  # add dependencies to pseudo-target
  add_hpx_pseudo_dependencies(micro_bench.immune_system.${subdir}
                              ${subdir}_exe)
endforeach()


##################################################################


