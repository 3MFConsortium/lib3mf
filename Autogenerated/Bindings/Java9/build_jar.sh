#!/bin/bash
set -euxo pipefail

cd "$(dirname "$0")"
echo "Download JNA"
[ -f jna-5.5.0.jar ] || curl -O https://repo1.maven.org/maven2/net/java/dev/jna/jna/5.5.0/jna-5.5.0.jar

echo "Compile Java Bindings"
javac -classpath *.jar lib3mf/*

echo "Create JAR"
jar cvf lib3mf-2.5.0.jar lib3mf
