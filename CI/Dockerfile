FROM multiarch/ubuntu-debootstrap:powerpc-xenial
RUN apt-get update
RUN apt-get -y install git cmake g++
ADD .. lib3mf-repo
ADD script.sh ./script.sh
ENTRYPOINT ["sh", "script.sh"]
