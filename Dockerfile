FROM debian:latest

RUN apt-get update
RUN apt-get install -y build-essentials cmake libqt5core5a libqt5gui5 libqt5widgets5 libqt5charts5-dev libqt5sql5 libqt5network5 zlib1g-dev

WORKDIR /tmp/logdoctor
COPY . .
RUN bash -c "scripts/LINUX_build_install.sh"

WORKDIR /
RUN rm -r /tmp/logdoctor

LABEL logdoctor.version=2.04

ENTRYPOINT /usr/bin/logdoctor
EOF
