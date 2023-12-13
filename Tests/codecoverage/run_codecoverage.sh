cd build
lcov --capture --initial --directory --ignore-errors . --output-file Test_CPP_Bindings_base.info

./Test_CPP_Bindings

lcov --directory . --capture --ignore-errors --output-file Test_CPP_Bindings_run.info
lcov --add-tracefile Test_CPP_Bindings_base.info --add-tracefile Test_CPP_Bindings_run.info --output-file Test_CPP_Bindings.info

TARGETDIR=`dirname \`pwd\``

lcov --remove Test_CPP_Bindings.info -o Test_CPP_Bindings_filtered.info "$TARGETDIR/Tests/*" "$TARGETDIR/Libraries/*" "$TARGETDIR/submodules/*" "/Applications/*" \
  "$TARGETDIR/Autogenerated/Bindings/C/*" \
  "$TARGETDIR/Autogenerated/Bindings/CDynamic/*" \
  "$TARGETDIR/Autogenerated/Bindings/CSharp/*" \
  "$TARGETDIR/Autogenerated/Bindings/CppDynamic/*" \
  "$TARGETDIR/Autogenerated/Bindings/Go/*" \
  "$TARGETDIR/Autogenerated/Bindings/NodeJS/*" \
  "$TARGETDIR/Autogenerated/Bindings/Python/*"


genhtml --output-directory ./codecoverage --title "lib3mf Test Coverage" --function-coverage --legend Test_CPP_Bindings_filtered.info   

zip -r codecoverage.zip ./codecoverage/
