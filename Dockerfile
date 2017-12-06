from    ubuntu:bionic

env     CC=clang
env     CXX=clang++

run     apt-get -yq update
run     apt-get install -yq build-essential autoconf libtool git cmake clang \
                            autoconf automake libtool curl make unzip \
                            libgoogle-glog-dev libgflags-dev libboost-all-dev

workdir /root/
run     git clone https://github.com/google/googletest.git && mkdir -p /root/googletest/build && \
        cd /root/googletest/build &&  cmake ../ && make -j4 && make install

add     . /root/learn_stl
run     mkdir -p /root/learn_stl/build
workdir /root/learn_stl/build
run     rm -rf * && cmake ../ && make -j4 && make test

