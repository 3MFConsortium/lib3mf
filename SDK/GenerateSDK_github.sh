#!/bin/bash
export SDKARTIFACT=build/lib3mf_sdk
export SDKSOURCE=SDK

export LOCATION="build"

export OUTFILE="lib3mf_sdk.zip"

function failed {
  echo "$1" 1>&2
  exit 1;
}

echo Clean artifacts-folder $SDKARTIFACT
rm -rf $SDKARTIFACT
mkdir $SDKARTIFACT

echo Copy License, Readme, Examples and Bindings
cat LICENSE > $SDKARTIFACT/License.txt & cat $SDKSOURCE/Credits.txt >> $SDKARTIFACT/License.txt
cp $SDKSOURCE/Readme.md $SDKARTIFACT
cp -r $SDKSOURCE/Examples $SDKARTIFACT/Examples
cp -r $LOCATION/Bindings $SDKARTIFACT

# download from readthedocs or build locally using sphinx
# cp ../Lib3MF-1.pdf $SDKARTIFACT/Doc/.

mkdir -p $SDKARTIFACT/Bin || failed "Error creating Bin-folder"
mkdir -p $SDKARTIFACT/Lib || failed "Error creating Lib-folder"

echo Copy binaries
cp $LOCATION/lib3mf.dll/lib3mf.dll $SDKARTIFACT/Bin/ || failed "Error copying binary"
cp $LOCATION/lib3mf.so/lib3mf.so.2 $SDKARTIFACT/Bin/lib3mf.so || failed "Error copying binary"
cp $LOCATION/lib3mf.dylib/lib3mf.dylib $SDKARTIFACT/Bin/ || failed "Error copying binary"
cp $LOCATION/lib3mf.lib/lib3mf.lib $SDKARTIFACT/Lib/ || failed "Error copying binary"

echo Generate version.txt
VERSIONTXT="$SDKARTIFACT/version.txt"
echo VERSIONTXT = $VERSIONTXT
echo "[lib3mf_sdk]" > $VERSIONTXT
echo "BuildNumber = "`git rev-parse HEAD` >> $VERSIONTXT
echo "GITBranch = "`git rev-parse --abbrev-ref HEAD` >> $VERSIONTXT
echo "GITRevision = "`git rev-parse HEAD` >> $VERSIONTXT

echo Zip SDK artifacts
cd $SDKARTIFACT
zip -r ../$OUTFILE ./* || failed "Error zipping SDK"
