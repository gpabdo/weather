FROM ubuntu:16.04

ENV PACKAGE_NAME environment
ENV PACKAGE_VERSION 0.1

RUN apt-get update
RUN apt-get install -y ruby-dev build-essential

RUN gem install fpm

ENTRYPOINT fpm -s dir --package /package/environment.deb -v ${PACKAGE_VERSION} -t deb -n ${PACKAGE_NAME} /package/environment/environment.py=/usr/bin/environment.py /package/environment/environment.conf=/etc/environment.conf /package/service/environment.service=/etc/systemd/system/
