idf_component_register(
    SRCS
        "main.c"
        "eol/eol_protocol.c"
        "eol/protocol_test.c"
        "eol/eol_manager.c"
        "eol/eol_registry.c"
        "eol/eol_executor.c"
        "eol/eol_node.c"
        "eol/eol_gateway_link.c"
        "eol/eol_lora_transport.c"

    INCLUDE_DIRS
        "."
        "eol"

    PRIV_INCLUDE_DIRS
        "../../orbdrive-firmware/products/common/include"
        "../../orbdrive-firmware/framework/core/include"
        "../../orbdrive-firmware/framework/nfw_hal/include"
        "../../orbdrive-firmware/boards/orb_drive_main"
)