#!/usr/bin/env bash


if [ "$#" -eq 0 ]; then
    ./build.sh plugins
    ./build.sh build

elif [ "$1" = "build" ]; then
    docker-compose up -d
    docker-compose exec dev ./fbt

elif [ "$1" = "plugins" ] || [ "$1" = "plugin" ]; then
    docker-compose exec dev ./fbt plugin_dist FIRMWARE_APP_SET=ext_apps

elif [ "$1" = "flash" ] || [ "$1" = "qflipper" ]; then
         cd dist/f7-C
         qFlipper

elif [ "$1" = "debug" ]; then
        script -e -f -a session.log -c "sudo screen /dev/ttyACM0 9600"

elif [ "$1" = "clean" ]; then
    # Clean by restarting the docker container
    docker-compose down
    echo "Sudo is required to remove the build directory..."
    sudo rm -rf ./dist ./build

elif [ "$1" = "help" ]; then
    echo "./build.sh [args]"
    echo "[NOARGS] - Runs 'plugin' and 'build' steps"
    echo "build - Compile using docker"
    echo "plugin - Build 'ext_apps' FAPs"
    echo "flash - Open up qFlipper"
    echo "qflipper - Open up qFlipper"
    echo "clean - Shutdown the docker container (clears build cache)"
    echo "debug - Connect to flipper with serial over usb and save session to 'session.log'"
    echo "help - Shows this menu"
else
    echo "Invalid arguments!"
fi
