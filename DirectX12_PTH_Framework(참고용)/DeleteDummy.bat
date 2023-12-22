#echo off
rmdir	/S /q ipch
rmdir	/S /q .vs
rmdir	/S /q Debug
rmdir	/a /q .vc
rmdir	/S /q .\Client\Default\Debug
rmdir	/S /q .\Client\Default\x64
rmdir	/S /q .\Engine\Default\Debug
rmdir	/S /q .\Engine\Default\x64
del		*.VC.db
del		 .\Engine\Bin\*.cso
del		 .\Tool\Bin\*.cso
del		 .\Client\Bin\*.cso
del		 .\Engine\Bin\*.pdb
del		 .\Tool\Bin\*.pdb
del		 .\Client\Bin\*.pdb
del		 .\Tool\Bin\*.exe
del		 .\Client\Bin\*.exe