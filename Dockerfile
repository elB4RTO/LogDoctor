FROM debian:latest

RUN apt-get update
RUN apt-get install -y build-essential cmake zlib1g-dev qttools5-dev-tools libqt5core5a libqt5gui5 libqt5widgets5 libqt5charts5-dev libqt5sql5 libqt5network5 qttools5-dev
RUN apt-get clean

WORKDIR /tmp/logdoctor
COPY . .
RUN chmod +x scripts/DOCKER_private.sh
RUN bash -c "scripts/DOCKER_private.sh"

WORKDIR /
RUN rm -rf /tmp/logdoctor

LABEL logdoctor.version=2.05

ENTRYPOINT /usr/bin/logdoctor
