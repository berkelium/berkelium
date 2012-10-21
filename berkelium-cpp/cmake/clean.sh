#!/bin/bash

cat .gitignore | while read line; do
    rm -rf $line
done