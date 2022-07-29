#!/usr/bin/env bash


if [ "$#" -eq 0 ]; then
    #Compile the code
    docker-compose up -d
    docker-compose exec dev ./fbt

elif [ "$1" = "flash" ] || [ "$1" = "qflipper" ]; then
         cd dist/f7-C
         ../../qFlipper.appimage
elif [ "$1" = "debug" ]; then
        script -e -f -a session.log -c "sudo screen /dev/ttyACM0 9600"
elif [ "$1" = "help" ]; then
    echo "./build.sh [args]"
    echo "build - Compile using docker"
    echo "flash - Open up qFlipper"
    echo "qflipper - Open up qFlipper"
    echo "debug - Connect to flipper with serial over usb and save session to 'session.log'"
    echo "help - Shows this menu"
else
    echo "Invalid arguments!"
fi
