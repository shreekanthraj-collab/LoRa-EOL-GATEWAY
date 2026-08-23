#include "eol_registry.h"

static const EolTestDefinition_t s_test_registry[] = {
	{ EOL_N01_BOOT_HEALTH,        EOL_GROUP_NODE,    "N01 Boot / basic health" },
	{ EOL_N02_FIRMWARE_IDENTITY,  EOL_GROUP_NODE,    "N02 Firmware identity" },
	{ EOL_N03_BOARD_IDENTITY,     EOL_GROUP_NODE,    "N03 Board identity" },
	{ EOL_N04_STARTUP_FAULT,      EOL_GROUP_NODE,    "N04 Startup fault state" },
	{ EOL_N05_INA226,              EOL_GROUP_NODE,    "N05 INA226" },
	{ EOL_N06_MOTOR_VALVE,         EOL_GROUP_NODE,    "N06 Motor / valve" },
	{ EOL_N07_MODBUS_FLOW1,        EOL_GROUP_NODE,    "N07 Modbus Flow 1 presence" },
	{ EOL_N08_MODBUS_FLOW2,        EOL_GROUP_NODE,    "N08 Modbus Flow 2 presence" },
	{ EOL_N09_MODBUS_OVERFLOW,     EOL_GROUP_NODE,    "N09 Modbus Overflow presence" },
	{ EOL_N10_NODE_LORA_TX,        EOL_GROUP_NODE,    "N10 Node LoRa TX" },

	{ EOL_G01_SHARED_SPI,          EOL_GROUP_GATEWAY, "G01 Shared SPI" },
	{ EOL_G02_RADIO0_IDENTITY,     EOL_GROUP_GATEWAY, "G02 Radio 0 identity" },
	{ EOL_G03_RADIO0_RESET,        EOL_GROUP_GATEWAY, "G03 Radio 0 RESET" },
	{ EOL_G04_RADIO0_BUSY,         EOL_GROUP_GATEWAY, "G04 Radio 0 BUSY" },
	{ EOL_G05_RADIO0_DIO1,         EOL_GROUP_GATEWAY, "G05 Radio 0 DIO1" },
	{ EOL_G06_RADIO1_IDENTITY,     EOL_GROUP_GATEWAY, "G06 Radio 1 identity" },
	{ EOL_G07_RADIO1_RESET,        EOL_GROUP_GATEWAY, "G07 Radio 1 RESET" },
	{ EOL_G08_RADIO1_BUSY,         EOL_GROUP_GATEWAY, "G08 Radio 1 BUSY" },
	{ EOL_G09_RADIO1_DIO1,         EOL_GROUP_GATEWAY, "G09 Radio 1 DIO1" },
	{ EOL_G10_RADIO0_RXTX_CONTROL, EOL_GROUP_GATEWAY, "G10 Radio 0 RX/TX control" },
	{ EOL_G11_RADIO1_RXTX_CONTROL, EOL_GROUP_GATEWAY, "G11 Radio 1 RX/TX control" },
	{ EOL_G12_RADIO0_TXRX,         EOL_GROUP_GATEWAY, "G12 Radio 0 TX/RX" },
	{ EOL_G13_RADIO1_TXRX,         EOL_GROUP_GATEWAY, "G13 Radio 1 TX/RX" },
	{ EOL_G14_RADIO_CS_ISOLATION,  EOL_GROUP_GATEWAY, "G14 Radio CS isolation" },
	{ EOL_G15_DUAL_CHANNEL_LORA,   EOL_GROUP_GATEWAY, "G15 Dual-channel LoRa" },

	{ EOL_S01_OLED,                EOL_GROUP_SYSTEM,   "S01 OLED" },
	{ EOL_S02_USB_LOCAL_HOST,      EOL_GROUP_SYSTEM,   "S02 USB / local host" },
	{ EOL_S03_OVERALL_RESULT,      EOL_GROUP_SYSTEM,   "S03 Overall EOL result" }
};

const EolTestDefinition_t *eol_registry_get(EolTestId_t id)
{
	if ((unsigned)id >= EOL_TEST_ID_COUNT) {
		return NULL;
	}

	return &s_test_registry[id];
}

size_t eol_registry_count(void)
{
	return sizeof(s_test_registry) / sizeof(s_test_registry[0]);
}
