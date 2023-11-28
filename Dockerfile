FROM ubuntu:18.04

ENV TZ=Etc/UTC

RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install -y build-essential
RUN apt-get install -y mingw-w64
RUN apt-get install -y mesa-common-dev
RUN apt-get install -y libglfw3-dev
RUN apt-get autoremove
