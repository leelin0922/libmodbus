export PATH=/home/lee/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin:$PATH
export CROSS_COMPILE=arm-linux-gnueabihf-
export ARCH=arm
# for ubuntu
# 1,sudo apt-get install libtool
# 2,sudo apt-get install autoconf
# 3,sudo apt-get install automake
./autogen.sh && ./configure --prefix=/home/lee/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf --host=arm-linux-gnueabihf && make clean && make && make install

sync
sleep 1
date
