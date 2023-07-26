FROM ubuntu:22.10

RUN apt-get update
RUN apt-get upgrade
RUN apt-get install -y build-essential
RUN apt-get install -y mingw-w64
RUN apt-get install -y mesa-common-dev
RUN apt-get autoremove