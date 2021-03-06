#!/bin/bash
#########################################################################
# File Name: detect.sh
# Author: loyxin
# mail: rafrozenluoxin@gmail.com
# Created Time: 2017-08-29 08:20:40
#########################################################################

length=`echo "$1+$2" | bc`

for var in 0.01 0.011 0.012 0.013 0.014 0.015 0.016 0.017 0.018 0.019 0.02 0.021 \
0.022 0.023 0.024 0.025 0.026 0.027 0.028 0.029 0.03 0.031 0.032 \
0.033 0.034 0.035 0.036 0.037 0.038 0.039 0.04 0.041 0.042 0.043 
do 
    cd ./$var 
    python ../detect.py $1 $2 $3
    python ../cal_aver_var.py $1 $2 $3
    cd ..
done
