#!/bin/bash

# Copyright (c) 2012 The Berkelium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

#tmp=$(mktemp)
#/usr/bin/xsltproc api/api-all.xslt api/api.xml > $tmp
#chmod +x $tmp
#cat $tmp
#$tmp
#rm $tmp

/usr/bin/xsltproc api/api-all.xslt api/api.xml | while read line; do
	echo $line
	bash -c "$line" || exit $?
done
