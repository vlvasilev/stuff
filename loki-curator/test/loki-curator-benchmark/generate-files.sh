#!/bin/bash

DIR=$1
# INODE_THRESHOLD_PERCENTAGE=$2

# if [ ! -d $DIR ]
# then 
#     echo "$DIR is not a directory"
#     exit 1
# fi

# total_inodes=$(df -i $DIR | awk 'NR > 1 {print $2}')
# used_inodes=$(df -i $DIR | awk 'NR > 1 {print $3}')
# files_to_generate=$(printf %.0f $(echo "scale=2; ($total_inodes*($INODE_THRESHOLD_PERCENTAGE/100))-$used_inodes" | bc))
<<<<<<< HEAD
files_to_generate=20000
# echo "total_inodes: $total_inodes used_inodes: $used_inodes files_to_generate: $files_to_generate"
x=1
timestamp=$(date +%s)
while [ $x -le $files_to_generate ]
do
    echo "some little content" > "${DIR}/file$x-$timestamp"
    x=$(( $x + 1 ))
=======
files_to_generate=20
# echo "total_inodes: $total_inodes used_inodes: $used_inodes files_to_generate: $files_to_generate"
x=1
# timestamp=$(date +%y%m%d-%Hh%Mm%Ss)
# unix_timestamp=$(date +%s)
while [ $x -le $files_to_generate ]
do
    echo "some little content" > "${DIR}/file$x-$(date +%y%m%d-%Hh%Mm%Ss-%s)"
    x=$(( $x + 1 ))
    sleep 2
    # unix_timestamp=$(date +%s)
    # timestamp=$(date +%y%m%d-%Hh%Mm%Ss)
>>>>>>> f21d90c (make revendor)
done
