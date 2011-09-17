@ECHO off 
cls 
color 0A 
ECHO 开始拷贝开发库
ECHO ========================================================== 

copy OpenXP\程序调试\Include\*.h 开发库\Include

copy OpenXP\界面控件\Include\*.h 开发库\Include
copy OpenXP\界面控件\Include\*.lib 开发库\Include

copy OpenXP\模板组件\Include\*.h 开发库\Include
copy OpenXP\模板组件\Include\*.lib 开发库\Include
copy OpenXP\模板组件\Include\paintlib\*.h 开发库\Include\paintlib
copy OpenXP\模板组件\Include\paintlib\*.cpp 开发库\Include\paintlib
copy OpenXP\模板组件\Include\paintlib\*.lib 开发库\Include\paintlib

copy OpenXP\数据库组件\Include\*.h 开发库\Include
copy OpenXP\数据库组件\Include\*.lib 开发库\Include
copy OpenXP\数据库组件\Include\*.dll 开发库\Include

copy OpenXP\文件操作\Include\*.h 开发库\Include
copy OpenXP\文件操作\MDFile\*.h 开发库\Include\MDFile
copy OpenXP\文件操作\MDFile\*.lib 开发库\Include\MDFile
copy OpenXP\文件操作\xml\*.h 开发库\Include\xml
copy OpenXP\文件操作\xml\*.lib 开发库\Include\xml
copy OpenXP\文件操作\zip\*.h 开发库\Include\zip
copy OpenXP\文件操作\zlib\*.h 开发库\Include\zlib
copy OpenXP\文件操作\zlib\*.lib 开发库\Include\zlib

copy OpenXP\下载组件\Include\*.h 开发库\Include

copy OpenXP\系统组件\Include\*.h 开发库\Include

copy OpenXP\音乐组件\Include\*.h 开发库\Include
copy OpenXP\音乐组件\IrrKlang\*.* 开发库\Include\IrrKlang

copy OpenXP\链接库\*.lib 开发库\Lib

copy OpenXP\程序调试\DebuggingExportHeader.h 开发库
copy OpenXP\界面控件\UIExportHeader.h 开发库
copy OpenXP\模板组件\TemplateExportHeader.h 开发库
copy OpenXP\数据库组件\DataBaseExportHeader.h 开发库
copy OpenXP\文件操作\FileExportHeader.h 开发库
copy OpenXP\下载组件\DownLoadExportHeader.h 开发库
copy OpenXP\系统组件\SystemExportHeader.h 开发库
copy OpenXP\音乐组件\SoundExportHeader.h 开发库

copy 开发库\include\*.lib 开发库\Lib
copy 开发库\include\paintlib\*.lib 开发库\Lib
copy 开发库\include\MDFile\*.lib 开发库\Lib
copy 开发库\include\xml\*.lib 开发库\Lib
copy 开发库\include\zip\*.lib 开发库\Lib
copy 开发库\include\zlib\*.lib 开发库\Lib
copy 开发库\include\IrrKlang\*.lib 开发库\Lib

ECHO 删除多余的lib
del  开发库\include\*.lib /f /s /q /a

ECHO 拷贝运行库
copy OpenXP\bin\*.dll 运行库

ECHO ========================================================== 
ECHO 拷贝结束 按任意键退出....
rem pause