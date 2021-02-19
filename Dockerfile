FROM ubuntu:latest
RUN apt upgrade -y && apt-get update -y
RUN apt install make -y
RUN apt install -y git
RUN apt	install gcc -y