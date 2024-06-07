# Copy this to root folder of lib3mf then build and run this image. 
FROM ubuntu:20.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install build-essential uuid-dev wget

RUN wget -qO- "https://cmake.org/files/v3.28/cmake-3.28.1-linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C /usr/local

ADD . lib3mf-repo

WORKDIR "/lib3mf-repo"

RUN cmake/GenerateMake.sh

WORKDIR "/lib3mf-repo/build"

RUN cmake --build . 

RUN ctest -V .
