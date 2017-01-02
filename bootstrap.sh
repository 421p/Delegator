#!/usr/bin/env bash

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y gcc-5 g++-5 libbz2-dev libssl-dev cmake g++ build-essential gcc-5-locales g++-5-multilib gcc-5-doc libstdc++6-5-dbg gcc-5-multilib libgcc1-dbg libgomp1-dbg libitm1-dbg libatomic1-dbg libasan2-dbg liblsan0-dbg libtsan0-dbg libubsan0-dbg libcilkrts5-dbg libmpx0-dbg libquadmath0-dbg libstdc++-5-doc cmake

sudo apt-get install build-essential software-properties-common -y
sudo apt-get install gcc-snapshot -y
sudo apt-get update
sudo apt-get install gcc-6 g++-6 -y
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6 && \
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 60 --slave /usr/bin/g++ g++ /usr/bin/g++-5;

wget https://cmake.org/files/v3.6/cmake-3.6.3.tar.gz
tar xf cmake-3.6.3.tar.gz

cd cmake-3.6.3
./bootstrap
make
sudo make install
cd ..

cmake .
make tests
