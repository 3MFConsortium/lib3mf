#!/bin/bash
export SDKARTIFACTS=lib3mf_sdk

if [ -n "$1" ]; then
	export LOCATION="$1"
else
	export LOCATION="remote"
fi

if [ -n "$2" ]; then
	export OUTFILE=$2
else
	export OUTFILE="lib3mf_sdk.zip"
fi

function failed {
  echo "$1" 1>&2
  exit 1;
}

#=Clean artifacts folder $SDKARTIFACTS=#
echo Clean artifacts-folder $SDKARTIFACTS
rm -rf $SDKARTIFACTS
mkdir $SDKARTIFACTS

#=Copy docs and examples=#
echo Copy docs and examples
cp -r Doc $SDKARTIFACTS
cp -r Examples $SDKARTIFACTS

#=Copy headers=#
echo Copy headers
mkdir -p $SDKARTIFACTS/Include
cp ../Include/Model/COM/NMR_DLLInterfaces.h $SDKARTIFACTS/Include/.

mkdir -p $SDKARTIFACTS/Include/Model
mkdir -p $SDKARTIFACTS/Include/Model/Classes
cp ../Include/Model/Classes/NMR_ModelTypes.h $SDKARTIFACTS/Include/Model/Classes/.
mkdir -p $SDKARTIFACTS/Include/Model/COM
cp ../Include/Model/COM/NMR_COMFactory.h $SDKARTIFACTS/Include/Model/COM/.
cp ../Include/Model/COM/NMR_COMVersion.h $SDKARTIFACTS/Include/Model/COM/.
cp ../Include/Model/COM/NMR_COMInterfaces.h $SDKARTIFACTS/Include/Model/COM/.

mkdir -p $SDKARTIFACTS/Include/Common
mkdir -p $SDKARTIFACTS/Include/Common/Platform
cp ../Include/Common/Platform/NMR_COM_Native.h $SDKARTIFACTS/Include/Common/Platform/.
cp ../Include/Common/Platform/NMR_SAL.h $SDKARTIFACTS/Include/Common/Platform/.
cp ../Include/Common/Platform/NMR_WinTypes.h $SDKARTIFACTS/Include/Common/Platform/.
cp ../Include/Common/Platform/NMR_COM_Emulation.h $SDKARTIFACTS/Include/Common/Platform/.

#=Obtain latest lib3MF-shared libraries=#
echo Obtain latest lib3MF-shared libraries
mkdir -p $SDKARTIFACTS/Lib || failed "Error creating Lib-folder"

function obtainlibrarycmd {
  if [ "$LOCATION" == "remote" ]; then
	  echo curl -s --retry 5 -m90 BUILDSERVER/artifact/artifacts/lib3MF_$1.$2 -o $SDKARTIFACTS/Lib/lib3MF.$2 || failed "Error obtaining latest lib3MF.$2"
	  curl -s --retry 5 -m90 BUILDSERVER/artifact/artifacts/lib3MF_$1.$2 -o $SDKARTIFACTS/Lib/lib3MF.$2 || failed "Error obtaining latest lib3MF.$2"
	  FS=`stat --printf="%s" $SDKARTIFACTS/Lib/lib3MF.$2`
	  if [ "$FS" -lt "1000" ]; then
		failed "Error obtaining latest lib3MF.$2. File size too small."
	  fi
  else
    cp $LOCATION/lib3MF_$1.$2 $SDKARTIFACTS/Lib/lib3MF.$2 || failed "Error obtaining latest lib3MF.$2"
  fi

}
obtainlibrarycmd "linux64" "so"
obtainlibrarycmd "win64" "dll"
obtainlibrarycmd "win64" "lib"
obtainlibrarycmd "darwin" "dylib"

#=Generate version.txt=#
VERSIONTXT="$SDKARTIFACTS/version.txt"
echo VERSIONTXT = $VERSIONTXT
echo "[Lib3MF_SDK]" > $VERSIONTXT
echo "BuildNumber = "$BUILD_NUMBER >> $VERSIONTXT
if [ -n "${GIT_BRANCH#*/}" ]; then
	echo "GITBranch = ${GIT_BRANCH#*/}" >> $VERSIONTXT
else
	echo "GITBranch = "`git rev-parse --abbrev-ref HEAD` >> $VERSIONTXT
fi
echo "GITRevision = "`git rev-parse HEAD` >> $VERSIONTXT

#=Zip SDK artifacts=#
zip -r $OUTFILE $SDKARTIFACTS/* || failed "Error zipping SDK"


