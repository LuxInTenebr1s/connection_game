cmake_minimum_required(VERSION 3.13.1)

set(BOARD nrf52832_mdk)
set(DTC_OVERLAY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/e73-tbb.overlay)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(connection_game)

target_sources(app PRIVATE src/main.c)
