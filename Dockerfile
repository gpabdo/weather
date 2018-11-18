FROM ubuntu:16.04

ENV PACKAGE_NAME environment
ENV PACKAGE_VERSION 0.1

RUN apt-get update
RUN apt-get install -y ruby-dev build-essential

RUN gem install fpm

ENTRYPOINT fpm \
--package /package/environment.deb \
-v ${PACKAGE_VERSION} \
-t deb \
-n ${PACKAGE_NAME} \
-d python-pip \
-s dir /package/service/arduino.py=/usr/bin/environment/arduino.py 
