#!/bin/bash

function do_test() {
    echo "=== Testing $* ==="
    $*
    if [ $? == 0 ] ; then
	echo "+++ $* Passed +++"
    else
	echo "--- $* Failed ---"
    fi
}
do_test make clean

do_test make -j16 pagerank.exe

do_test ./pagerank.exe cit-Patents.mmio
