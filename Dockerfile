FROM ubuntu:latest
RUN apt upgrade -y && apt-get update -y
RUN apt install make -y
RUN apt install -y git
RUN apt	install gcc -y
RUN apt install nasm -y
RUN apt install zsh -y
RUN apt install wget -y
RUN wget https://github.com/robbyrussell/oh-my-zsh/raw/master/tools/install.sh -O - | zsh || true\
CMD zsh