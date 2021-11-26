@ECHO off

CALL config.cmd

pushd %MyWorkDir%

%MyProjectDir%\%TypeBuild%\%MyExeName%.exe -n 4

popd