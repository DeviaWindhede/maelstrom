mkdir bin\
mkdir bin\editor\
mkdir bin\launcherDebug\
mkdir bin\release\

copy dependencies\dll\* bin\debug\
copy dependencies\debug\dll\* bin\debug\

copy dependencies\dll\* bin\release\
copy dependencies\release\dll\* bin\release\

call "premake/premake5" --file=source/premake5.lua vs2022
pause
