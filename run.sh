#!/bin/bash

out=`(./Lab5)`
echo $out
if [[ $out == *"!!!FAILURES!!!"* ]]; then
  exit 1;
fi


