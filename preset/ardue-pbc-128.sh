#!/bin/bash 
ARDUINO=${HOME}/.arduino15/packages/arduino/
CC="${ARDUINO}/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-gcc -nostdlib" CXX="${ARDUINO}/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-g++ -nostdlib" LINK="-mthumb -Wl,-gc-sections" COMP="-O2 -ggdb -DF_CPU=84000000L -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections" cmake -DCMAKE_SYSTEM_NAME="Generic" -DARCH=ARM -DWORD=32 -DOPSYS=DUINO -DSEED=LIBC -DSHLIB=OFF -DSTBIN=ON -DTIMER=HREAL -DWITH="DV;BN;FP;EP;EC;CP;MD;FPX;EPX;PP;PC" -DBENCH=0 -DTESTS=0 -DCHECK=off -DVERBS=off -DSTRIP=on -DQUIET=off -DARITH=easy -DBN_METHD="COMBA;COMBA;BASIC;BASIC;STEIN;BASIC" -DBN_PRECI=256 -DBN_MAGNI=DOUBLE -DFP_PRIME=256 -DFP_QNRES=off -DFP_METHD="INTEG;COMBA;COMBA;MONTY;EXGCD;SLIDE" -DEP_ENDOM=on -DEP_PLAIN=off -DEP_SUPER=off -DEC_ENDOM=on -DEC_METHD="PRIME" -DMD_METHD=SHONE -DFPX_METHD="INTEG;INTEG;LAZYR" -DPP_METHD="LAZYR;OATEP" $1

