FROM debian:latest

RUN apt-get update
RUN apt-get install -y build-essential cmake zlib1g-dev qt6-tools-dev qt6-tools-dev-tools qt6-charts-dev libqt6core6 libqt6gui6 libqt6widgets6 libqt6charts6 libqt6sql6 libqt6network6
RUN apt-get clean

WORKDIR /tmp/logdoctor
COPY . .
RUN chmod +x scripts/DOCKER_private.sh
RUN bash -c "scripts/DOCKER_private.sh"

WORKDIR /
RUN rm -rf /tmp/logdoctor

LABEL logdoctor.version=4.00

ENTRYPOINT /usr/bin/logdoctor
