#!/bin/bash

if [ $# -ne 2 ]
	then
	echo "2 paramaters are required!"
	exit 1
fi

CRAWLER_DIR="$1"/
INDEX_FILE="$2"

if [ ! -d "$CRAWLER_DIR" ]
	then
	echo "Please provide an existing directory"
	exit 1
	if [ ! -r "$CRAWLER_DIR" ]
		then
		echo "Please provide a readable directory"
		exit 1
	fi
fi

if [ ! -r "$INDEX_FILE" ]
	then
	echo "Please provide an existing/readable index file"
	exit 1
fi

# Check for proper inputs
./querier faultyDir faultyIndexFile
./querier a b c d
./querier

# Check for different inputs
./querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index <<< "include"
./querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index <<< "include palmer"
./querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index <<< "include or palmer"
./querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index <<< "include or xia"
./querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index <<< "include palmer or xia"
./querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index <<< "include or xia palmer"
./querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index <<< "include or xia or palmer"