# Install docker
```
$ sudo apt-get install -y\
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common

$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

$ echo "deb [arch=armhf] https://download.docker.com/linux/debian \
     $(lsb_release -cs) stable" | \
    sudo tee /etc/apt/sources.list.d/docker.list

$ sudo apt-get update

$ sudo apt-get install -y docker-ce

$ sudo usermod -aG docker $USER
```

# Launch the weather docker
```
$ echo "LOCATION=Bedroom" >> ~/.bashrc
$ echo "SITE=pad" >> ~/.bashrc
$ . ./.bashrc
$ docker run -d \
     --restart always \
     --name weather \
     -v /dev/:/host/ \
     -e LOCATION=$LOCATION \
     -e SITE=$SITE \
     -e DEVICE=/host/ttyACM0 \
     --privileged \
     eventngine/environment:arm-0.0.1
```
