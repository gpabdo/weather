#!/bin/bash

INSTALL_HOME=/usr/bin/environment/
USER=environment

useradd $USER
usermod -aG tty $USER

mkdir -p $INSTALL_HOME 
cp ./environment/environment.py $INSTALL_HOME/
cp ./environment/config $INSTALL_HOME
chown  $USER $INSTALL_HOME

cp ./service/environment.service /etc/systemd/system/

pip install -r ./environment/requirements.txt

service environment restart
