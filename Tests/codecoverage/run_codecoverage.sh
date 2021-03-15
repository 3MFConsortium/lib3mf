cd build

lcov --zerocounters  --directory .

./Test_CPP_Bindings

lcov --directory . --capture --output-file Test_CPP_Bindings.info

TARGETDIR=`dirname \`pwd\``

lcov --remove Test_CPP_Bindings.info -o Test_CPP_Bindings_filtered.info "$TARGETDIR/Tests/*" "$TARGETDIR/Source/Libraries/*" "$TARGETDIR/Include/Libraries/*" "/Applications/*"

genhtml --output-directory ./codecoverage --title "lib3mf Test Coverage" --function-coverage --legend Test_CPP_Bindings_filtered.info   

zip -r codecoverage.zip ./codecoverage/