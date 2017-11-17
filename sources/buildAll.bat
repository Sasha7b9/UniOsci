@echo off

echo %TIME%
echo Compile Boot53   , 1 / 6 ...

c:\Keil_v5\UV4\UV4 -b S8-53\Boot53.uvprojx -j0 -z -o Boot53.out
if ERRORLEVEL 1 goto BadBoot53

@echo %TIME%
echo Compile Device53 , 2 / 6 ...

:FineBoot53
c:\Keil_v5\UV4\UV4 -b S8-53\Device53.uvprojx -j0 -z -o Device53.out
if ERRORLEVEL 1 goto BadDevice53

@echo %TIME%
echo Compile Boot54   , 3 / 6 ...

:FineDevice53
c:\Keil_v5\UV4\UV4 -b S8-54\Boot54.uvprojx -j0 -z -o Boot54.out
if ERRORLEVEL 1 goto BadBoot54

@echo %TIME%
echo Compile Device54 , 4 / 6 ...

:FineBoot54
c:\Keil_v5\UV4\UV4 -b S8-54\Device54.uvprojx -j0 -z -o Device54.out
if ERRORLEVEL 1 goto BadDevice54

@echo %TIME%
echo Compile Panel    , 5 / 6 ...

:FineDevice54
c:\Keil_v5\UV4\UV4 -b Kornet\Panel.uvprojx -j0 -z -o Panel.out
if ERRORLEVEL 1 goto BadPanel

@echo %TIME%
echo Compile Device   , 6 / 6 ...

:FinePanel
c:\Keil_v5\UV4\UV4 -b Kornet\Device.uvprojx -j0 -z -o Device.out
if ERRORLEVEL 1 goto BadDevice

:FineDevice


exit

:BadBoot53
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop S8-53\Boot53.out -n1000 -x0 -y0
goto FineBoot53

:BadDevice53
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop S8-53\Device53.out -n1000 -x0 -y0
goto FineDevice53

:BadBoot54
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop S8-54\Boot54.out -n1000 -x0 -y0
goto FineBoot54

:BadDevice54
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop S8-54\Device54.out -n1000 -x0 -y0
goto FineDevice54

:BadPanel
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop Kornet\Panel.out -n1000 -x0 -y0
goto FinePanel

:BadDevice
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop Kornet\Device.out -n1000 -x0 -y0
goto FineDevice
