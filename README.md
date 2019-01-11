docker run -d --restart always --name weather -v /dev/:/host/ -e LOCATION=Livingroom -e DEVICE=/host/ttyACM0 -e SITE=pad --privileged eventngine/weather:arm-0.0.1
