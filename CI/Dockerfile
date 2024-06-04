FROM registry.access.redhat.com/ubi8/ubi-minimal:latest

LABEL maintainer="pradeep.gangatharan@autodesk.com"

ENV GCCTOOLSET=gcc-toolset-12

RUN \
    microdnf update -y && \
    microdnf -y install --nodocs \
    wget \
    which \
    libuuid-devel \
    glibc-langpack-en \
    tar \
    gzip \
    zip \
    rpm-build \
    ${GCCTOOLSET} \
    && microdnf clean all

ENV LD_LIBRARY_PATH=/opt/rh/${GCCTOOLSET}/root/usr/lib64:/opt/rh/${GCCTOOLSET}/root/usr/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
ENV PKG_CONFIG_PATH=/opt/rh/${GCCTOOLSET}/root/usr/lib64/pkgconfig${PKG_CONFIG_PATH:+:${PKG_CONFIG_PATH}}
ENV PATH=/opt/rh/${GCCTOOLSET}/root/usr/bin${PATH:+:${PATH}}

# CMake
ARG CMAKE_VERSION=3.28.1
ADD "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz" /usr/tmp/
RUN tar xzf /usr/tmp/cmake-$CMAKE_VERSION-linux-x86_64.tar.gz -C /opt && rm /usr/tmp/cmake-$CMAKE_VERSION-linux-x86_64.tar.gz
ENV PATH /opt/cmake-$CMAKE_VERSION-linux-x86_64/bin:${PATH}


RUN ln -s /opt/cmake-$CMAKE_VERSION-linux-x86_64/bin/cmake /usr/bin/cmake3
RUN ln -s /opt/cmake-$CMAKE_VERSION-linux-x86_64/bin/cpack /usr/bin/cpack3
RUN ln -s /opt/cmake-$CMAKE_VERSION-linux-x86_64/bin/ctest /usr/bin/ctest3


RUN ldd --version
RUN cmake --version
RUN cmake3 --version
RUN gcc --version


ADD . lib3mf-repo

WORKDIR "/lib3mf-repo"

RUN chmod +x cmake/GenerateMake.sh

RUN cmake/GenerateMake.sh

WORKDIR "/lib3mf-repo/build"

RUN cmake --build . 

RUN ctest -V .

# Add this line to generate a ZIP package with cpack
RUN cpack -G ZIP -C Release

# Generate a debian package
RUN cpack -G DEB -C Release

# Generate a RPM package
RUN cpack -G RPM -C Release

WORKDIR "/../../"

RUN mkdir -p out

RUN cp ./lib3mf-repo/build/lib3mf.so.2 ./out/

RUN cp ./lib3mf-repo/build/lib3mf-*-Linux.zip ./out/

RUN cp ./lib3mf-repo/build/lib3mf-*-Linux.deb ./out/

RUN cp ./lib3mf-repo/build/lib3mf-*-Linux.rpm ./out/

RUN cd out && zip -r ../out.zip .







