# Build
```
$ docker build -t eventngine/environment:arm-0.0.2 .
$ docker push eventngine/environment:arm-0.0.2
```

# Install docker
```
$ sudo apt-get install -y\
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common \
    git

$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

$ echo "deb [arch=armhf] https://download.docker.com/linux/debian \
     $(lsb_release -cs) stable" | \
    sudo tee /etc/apt/sources.list.d/docker.list

$ sudo apt-get update

$ sudo apt-get install -y docker-ce

$ sudo usermod -aG docker pi
```

# Launch the weather docker
```
$ echo "LOCATION=Bedroom" >> ~/.bashrc
$ . ./.bashrc
$ docker run -d --restart always --name weather -v /dev/:/host/ -e LOCATION=$LOCATION -e DEVICE=/host/ttyACM0 -e API_URL=http://environment.the-collective-group.com -e SITE=$SITE --privileged eventngine/environment:arm-0.0.2 
```
