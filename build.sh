#!/usr/bin/env bash

# Compile the code
docker-compose up -d
docker-compose exec dev ./fbt

# Open up qFlipper (for flashing)
./qFlipper-x86_64.AppImage
