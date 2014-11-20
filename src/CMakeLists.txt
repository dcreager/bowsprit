# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------
# Copyright © 2011-2014, RedJack, LLC.
# All rights reserved.
#
# Please see the COPYING file in this distribution for license details.
# ----------------------------------------------------------------------

include_directories(${CMAKE_SOURCE_DIR}/include)

#-----------------------------------------------------------------------
# Build the library

set(LIBBOWSPRIT_SRC
    libbowsprit/logger.c
    libbowsprit/periodic.c
    libbowsprit/render.c
    libbowsprit/rotated-files.c
    libbowsprit/snapshot.c
    libbowsprit/stats.c
)

# Update the VERSION and SOVERSION properties below according to the following
# rules (taken from [1]):
#
# VERSION = current.revision.age
#
#   1. Start with a VERSION of `0.0.0` for each shared library.
#   2. Update VERSION only immediately before a public release of your software.
#      More frequent updates are unnecessary, and only guarantee that the
#      current interface number gets larger faster.
#   3. If the library source code has changed at all since the last update, then
#      increment `revision` (`c.r.a` becomes `c.r+1.a`).
#   4. If any interfaces have been added, removed, or changed since the last
#      update, increment `current`, and set `revision` to 0.
#   5. If any interfaces have been added since the last public release, then
#      increment `age`.
#   6. If any interfaces have been removed or changed since the last public
#      release, then set `age` to 0.
#
# SOVERSION should always equal `current-age`.
#
# Note that changing `current` means that you are releasing a new
# backwards-incompatible version of the library.  This has implications on
# packaging, so once an API has stabilized, these should be a rare occurrence.
#
# [1] http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html#Updating-version-info

add_library(libbowsprit SHARED ${LIBBOWSPRIT_SRC})
set_target_properties(libbowsprit PROPERTIES
    OUTPUT_NAME bowsprit
    VERSION 2.0.0
    SOVERSION 2)
target_link_libraries(libbowsprit
    ${CORK_LIBRARIES}
    ${CLOG_LIBRARIES}
)

install(TARGETS libbowsprit DESTINATION ${CMAKE_INSTALL_LIBDIR})

#-----------------------------------------------------------------------
# bowsprit-test

set(BOWSPRIT_TEST_SRC
    bowsprit-test/bowsprit-test.c
)

add_executable(bowsprit-test ${BOWSPRIT_TEST_SRC})
target_link_libraries(bowsprit-test libbowsprit)

#-----------------------------------------------------------------------
# Generate the pkg-config file

set(prefix ${CMAKE_INSTALL_PREFIX})
configure_file(bowsprit.pc.in ${CMAKE_CURRENT_BINARY_DIR}/bowsprit.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/bowsprit.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
