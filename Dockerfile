FROM python:2.7

ENV SITE='unkown'
ENV LOCATION='unknown'
ENV KAFKA='kafka.pad.the-collective-group.com:9092'
ENV DEVICE='/host/ttyUSB0'

COPY ./environment/requirements.txt /tmp/
RUN pip install -r /tmp/requirements.txt 

COPY ./environment/arduinoCommunication /usr/local/lib/python2.7/site-packages/arduinoCommunication

RUN mkdir /app
WORKDIR /app
COPY ./environment/environment.py /app/environment.py

ENTRYPOINT ["/usr/local/bin/python", "/app/environment.py"]
