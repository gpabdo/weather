FROM python:3.9

ENV SITE='unkown'
ENV LOCATION='unknown'
ENV API_URL='http://environment.the-collective-group.com' 
ENV DEVICE='/host/ttyUSB0'
ENV PYTHONWARNINGS="ignore:Unverified HTTPS request"


COPY ./environment/requirements.txt /tmp/
RUN pip install -r /tmp/requirements.txt 

COPY ./environment/arduinoCommunication /usr/local/lib/python3.9/site-packages/arduinoCommunication

RUN mkdir /app
WORKDIR /app
COPY ./environment/environment.py /app/environment.py

ENTRYPOINT ["/usr/local/bin/python", "-u", "/app/environment.py"]
