@eco lets get ready to coppy!! %1 in %3

@goto %3

:Debug

@goto %2

:DLL
@echo DLL
@echo Copy Headers to ..\..\OSG_Assignemnt_Build\include\%1
@mkdir ..\..\OSG_Assignemnt_Build\include\%1
@copy *.h ..\..\OSG_Assignemnt_Build\include\%1\.

@copy ..\x64\Debug\*.lib ..\..\OSG_Assignemnt_Build\lib\.
@copy ..\x64\Debug\*.dll ..\..\OSG_Assignemnt_Build\bin\

@goto end

:EXE
@echo EXE
@copy ..\x64\Debug\%1D.exe ..\..\OSG_Assignemnt_Build\bin\.

@goto end

:Release

@goto %2

:DLL
@echo DLL
@echo Copy Headers to ..\..\OSG_Assignemnt_Build\include\%1
@mkdir ..\..\OSG_Assignemnt_Build\include\%1
@copy *.h ..\..\OSG_Assignemnt_Build\include\%1\.

@copy ..\x64\Release\*.lib ..\..\OSG_Assignemnt_Build\lib\.
@copy ..\x64\Release\*.dll ..\..\OSG_Assignemnt_Build\bin\

@goto end

:EXE
@echo EXE
@copy ..\x64\Release\%1D.exe ..\..\OSG_Assignemnt_Build\bin\.

@goto end

:end
@echo copying %1 Complete