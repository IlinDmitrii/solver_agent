@ECHO off
rem if not defined CONFIG_DONE (
CALL config.cmd
rem )
SET NumProcs=4
SET MPIExecExe=%I_MPI_ONEAPI_ROOT%\bin\mpiexec.exe
SET MPIPPrefix="%MPIExecExe%" -n %NumProcs% -wdir %MyWorkDir% -envall -print-all-exitcodes 
SET PATH=c:\Program Files (x86)\Intel\oneAPI\mpi\latest\bin\release\;c:\Program Files (x86)\Intel\oneAPI\compiler\latest\windows\redist\intel64_win\compiler\;%PATH% 
rem I_MPI_DEBUG=6
pushd %MyWorkDir%
%MPIPPrefix% %MyProjectDir%\%TypeBuild%\%MyExeName%.exe 
popd
