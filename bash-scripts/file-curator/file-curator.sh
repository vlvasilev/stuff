#!/bin/bash

DIR=$1
MIN_FREE_INODE_THRESHOLD_PERCENTAGE=$2
TARGET_FREE_INODE_THRESHOLD_PERCENTAGE=$3
SLEEP=$4

while true;
do
    if [ ! -d $DIR ]
    then 
        echo "$DIR is not a directory"
        exit 1
    fi
    if [ $TARGET_FREE_INODE_THRESHOLD_PERCENTAGE -lt $MIN_FREE_INODE_THRESHOLD_PERCENTAGE ]
    then
        echo "Target free inodes persentage $TARGET_FREE_INODE_THRESHOLD_PERCENTAGE must be equal or greather than min free inode threshold $MIN_FREE_INODE_THRESHOLD_PERCENTAGE"
        exit 2
    fi

    total_inodes=$(df -i $DIR | awk 'NR > 1 {print $2}')
    free_inodes=$(df -i $DIR | awk 'NR > 1 {print $4}')
    min_free_inodes=$(printf %.0f $(echo "scale=2; ($total_inodes*($MIN_FREE_INODE_THRESHOLD_PERCENTAGE/100))" | bc))
    echo "total_inodes: $total_inodes free_inodes: $free_inodes min_free_inodes: $min_free_inodes"
    echo $(df -i $DIR)
    if [ $min_free_inodes -gt  $free_inodes ]
    then
       cd $DIR 
       inodes_to_delete=$(printf %.0f $(echo "scale=2; ($total_inodes*($TARGET_FREE_INODE_THRESHOLD_PERCENTAGE/100))-$free_inodes" | bc))
       echo "total_inodes: $total_inodes free_inodes: $free_inodes inodes_to_delete: $inodes_to_delete"
       ls $DIR -1t | tail -n -$inodes_to_delete | xargs -I {} rm -- {}
       df -i $DIR
    fi
    sleep $SLEEP
done