::
::  这是一个编译脚本
::  编译之前需确认：
::  1、./ChemInk/thrid_party下的文件组织满足./ChemInk/thrid_party/Structure.md
::  2、把 QT_MSVC_2017>=5.12.2 的bin目录加入环境变量
::  3、在 vs 拓展中设置qt_vs_tools的默认qt版本为QT_msvc2017
::  4、在 vs x64 native tools 命令行中运行这个脚本
msbuild ChemInk.sln /p:Configuration=Release -m
::  5、在 ./x64/Release文件夹下出现可执行文件
::  6、将 ./ChemInk/data 文件夹复制到可执行文件所在目录
::  可是为什么要用脚本呢，还是IDE方便（x
::