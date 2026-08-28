cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/../orbdrive-firmware/framework/core"
    "${CMAKE_CURRENT_LIST_DIR}/../orbdrive-firmware/framework/nfw_hal"
    "${CMAKE_CURRENT_LIST_DIR}/../orbdrive-firmware/framework/platform"
    "${CMAKE_CURRENT_LIST_DIR}/../orbdrive-firmware/products/common"

    "C:/Users/Shreekanth Raj/LORA-GATEWAY/components/gw_core"
    "C:/Users/Shreekanth Raj/LORA-GATEWAY/components/gw_platform"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(lora_eol_gateway)