import posixpath

# For more details on these options, run 'fbt -h'


# Default hardware target
TARGET_HW = 7

# Optimization flags
## Optimize for size
COMPACT = 1
## Optimize for debugging experience
DEBUG = 0

# Suffix to add to files when building distribution
# If OS environment has DIST_SUFFIX set, it will be used instead
<<<<<<< HEAD
DIST_SUFFIX = "RM420"
||||||| [FL-2764] SubGhz: fix CAME, Chamberlain potocol (#1650)
DIST_SUFFIX = "local"
=======
DIST_SUFFIX = "RM420FAP"
>>>>>>> unleashed

# Coprocessor firmware
COPRO_OB_DATA = "scripts/ob.data"

# Must match lib/STM32CubeWB version
COPRO_CUBE_VERSION = "1.13.3"

COPRO_CUBE_DIR = "lib/STM32CubeWB"

# Default radio stack
COPRO_STACK_BIN = "stm32wb5x_BLE_Stack_light_fw.bin"
# Firmware also supports "ble_full", but it might not fit into debug builds
COPRO_STACK_TYPE = "ble_light"

# Leave 0 to let scripts automatically calculate it
COPRO_STACK_ADDR = "0x0"

# If you override COPRO_CUBE_DIR on commandline, override this aswell
COPRO_STACK_BIN_DIR = posixpath.join(
    COPRO_CUBE_DIR,
    "Projects",
    "STM32WB_Copro_Wireless_Binaries",
    "STM32WB5x",
)

# Supported toolchain versions
FBT_TOOLCHAIN_VERSIONS = (" 10.3.",)

OPENOCD_OPTS = [
    "-f",
    "interface/stlink.cfg",
    "-c",
    "transport select hla_swd",
    "-f",
    "debug/stm32wbx.cfg",
    "-c",
    "stm32wbx.cpu configure -rtos auto",
    "-c",
    "init",
]

SVD_FILE = "debug/STM32WB55_CM4.svd"

# Look for blackmagic probe on serial ports and local network
BLACKMAGIC = "auto"

# Application to start on boot
LOADER_AUTOSTART = ""

FIRMWARE_APPS = {
    "default": [
        # Svc
        "basic_services",
        # Apps
<<<<<<< HEAD
        "basic_apps",
        "updater_app",
        "archive",
        # Custom Apps
        "custom_apps",
||||||| [FL-2764] SubGhz: fix CAME, Chamberlain potocol (#1650)
        "basic_apps",
        "updater_app",
        "storage_move_to_sd",
        "archive",
=======
        "main_apps",
        "system_apps",
>>>>>>> unleashed
        # Settings
<<<<<<< HEAD
        "passport",
        "system_settings",
        "about",
        "basic_plugins",
        # Custom Games
        "custom_games",
        # WIFI Plugins
        "wifi_plugins",
||||||| [FL-2764] SubGhz: fix CAME, Chamberlain potocol (#1650)
        "passport",
        "system_settings",
        "about",
        # Plugins
        "basic_plugins",
=======
        "settings_apps",
        # Stock plugins - no longer built into fw, now they're .faps
        # Yet you can still build them as a part of fw
        # "basic_plugins",
>>>>>>> unleashed
        # Debug
        # "debug_apps",
    ],
    "debug_pack": [
        # Svc
        "basic_services",
        # Apps
        "main_apps_default",
        "system_apps",
        # Settings
        "settings_apps",
        # Plugins
        # "basic_plugins",
        # Debug
        # "debug_apps",
    ],
    "ext_apps": [
        "basic_services",

         "external_apps"
    ],

}

FIRMWARE_APP_SET = "default"
