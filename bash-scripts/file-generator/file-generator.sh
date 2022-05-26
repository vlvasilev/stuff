#!/bin/sh

DIR=$1
INODE_THRESHOLD_PERCENTAGE=$2

if [ ! -d $DIR ]
then 
    echo "$DIR is not a directory"
fi

total_inodes=$(df -i $DIR | awk 'NR > 1 {print $2}')
used_inodes=$(df -i $DIR | awk 'NR > 1 {print $3}')
files_to_generate=$(printf %.0f $(echo "scale=2; ($total_inodes*($INODE_THRESHOLD_PERCENTAGE/100))-$used_inodes" | bc))

echo "total_: $total_inodes used_inodes: $used_inodes files_to_generate: $files_to_generate"
x=1
while [ $x -le $files_to_generate ]
do
  touch "${DIR}/file$x"
  x=$(( $x + 1 ))
done