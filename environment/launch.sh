#!/bin/bash

source ~/.bashrc

echo "Location set to: "$LOCATION
echo "Site set to: " $SITE

docker kill weather
docker rm weather

docker run -d \
     --restart always \
     --name weather \
     -v /dev/:/host/ \
     -e LOCATION=$LOCATION \
     -e SITE=$SITE \
     -e DEVICE=/host/ttyACM0 \
     -e API_URL=http://environment.the-collective-group.com \
     --privileged \
     eventngine/environment:arm-0.0.4
