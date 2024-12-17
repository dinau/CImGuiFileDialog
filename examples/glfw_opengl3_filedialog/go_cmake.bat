if exist ".build" goto make

rem for GCC
cmake -B .build -DSTATIC_BUILD=true -G"MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release && cd .build && make

rem for MSVC
rem cmake -B .build -DSTATIC_BUILD=true -DCMAKE_BUILD_TYPE=Release
goto exitProg

:make
cd .build && make
goto exitProg


:exitProg
