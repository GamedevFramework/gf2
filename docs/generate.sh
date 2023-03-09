#!/bin/sh -x

asciidoctor -v -D export *.adoc
cp -ar assets/* export/
