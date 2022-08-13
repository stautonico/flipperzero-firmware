#!/usr/bin/env bash


if [ "$#" -eq 0 ]; then
    ./build.sh plugins
    ./build.sh build

elif [ "$1" = "build" ]; then
    ./fbt

elif [ "$1" = "plugins" ] || [ "$1" = "plugin" ]; then
    ./fbt plugin_dist FIRMWARE_APP_SET=ext_apps

elif [ "$1" = "flash" ] || [ "$1" = "qflipper" ]; then
         cd dist/f7-C
         qFlipper

elif [ "$1" = "debug" ]; then
        script -e -f -a session.log -c "sudo screen /dev/ttyACM0 9600"

elif [ "$1" = "clean" ]; then
    rm -rf ./dist ./build ./assets/compiled

elif [ "$1" = "help" ]; then
    echo "./build.sh [args]"
    echo "[NOARGS] - Runs 'plugin' and 'build' steps"
    echo "build - Compile using fbt (flipper build tool)"
    echo "plugin - Build 'ext_apps' FAPs"
    echo "flash - Open up qFlipper"
    echo "qflipper - Open up qFlipper"
    echo "clean - Clears build cache"
    echo "debug - Connect to flipper with serial over usb and save session to 'session.log'"
    echo "help - Shows this menu"
else
    echo "Invalid arguments!"
fi
