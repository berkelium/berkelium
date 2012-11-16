#!/bin/bash

cat .gitignore | while read line; do
    [ "$line" == "eclipse" ] && continue
    rm -rf $line
done