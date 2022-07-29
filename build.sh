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
elif [ "$1" = "clean" ]; then
    # Clean by restarting the docker container
    docker-compose down
    echo "Sudo is required to remove the build directory..."
    sudo rm -rf ./dist ./build
elif [ "$1" = "help" ]; then
    echo "./build.sh [args]"
    echo "build - Compile using docker"
    echo "flash - Open up qFlipper"
    echo "qflipper - Open up qFlipper"
    echo "clean - Shutdown the docker container (clears build cache)"
    echo "debug - Connect to flipper with serial over usb and save session to 'session.log'"
    echo "help - Shows this menu"
else
    echo "Invalid arguments!"
fi
