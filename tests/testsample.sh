export PATH=$HOME/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin:$PATH
export CROSS_COMPILE=arm-linux-gnueabihf-
export ARCH=arm

arm-linux-gnueabihf-gcc testsample.c -I ../include/ -L ../lib/ -l modbus -o testsample
cp testsample /media/sf_share/7109/.

sync
sleep 1
date
