@ECHO off 
cls 
color 0A 
ECHO ��ʼ����������
ECHO ========================================================== 

copy OpenXP\�������\Include\*.h ������\Include

copy OpenXP\����ؼ�\Include\*.h ������\Include
copy OpenXP\����ؼ�\Include\*.lib ������\Include

copy OpenXP\ģ�����\Include\*.h ������\Include
copy OpenXP\ģ�����\Include\*.lib ������\Include
copy OpenXP\ģ�����\Include\paintlib\*.h ������\Include\paintlib
copy OpenXP\ģ�����\Include\paintlib\*.cpp ������\Include\paintlib
copy OpenXP\ģ�����\Include\paintlib\*.lib ������\Include\paintlib

copy OpenXP\���ݿ����\Include\*.h ������\Include
copy OpenXP\���ݿ����\Include\*.lib ������\Include
copy OpenXP\���ݿ����\Include\*.dll ������\Include

copy OpenXP\�ļ�����\Include\*.h ������\Include
copy OpenXP\�ļ�����\MDFile\*.h ������\Include\MDFile
copy OpenXP\�ļ�����\MDFile\*.lib ������\Include\MDFile
copy OpenXP\�ļ�����\xml\*.h ������\Include\xml
copy OpenXP\�ļ�����\xml\*.lib ������\Include\xml
copy OpenXP\�ļ�����\zip\*.h ������\Include\zip
copy OpenXP\�ļ�����\zlib\*.h ������\Include\zlib
copy OpenXP\�ļ�����\zlib\*.lib ������\Include\zlib

copy OpenXP\�������\Include\*.h ������\Include

copy OpenXP\ϵͳ���\Include\*.h ������\Include

copy OpenXP\�������\Include\*.h ������\Include
copy OpenXP\�������\IrrKlang\*.* ������\Include\IrrKlang

copy OpenXP\���ӿ�\*.lib ������\Lib

copy OpenXP\�������\DebuggingExportHeader.h ������
copy OpenXP\����ؼ�\UIExportHeader.h ������
copy OpenXP\ģ�����\TemplateExportHeader.h ������
copy OpenXP\���ݿ����\DataBaseExportHeader.h ������
copy OpenXP\�ļ�����\FileExportHeader.h ������
copy OpenXP\�������\DownLoadExportHeader.h ������
copy OpenXP\ϵͳ���\SystemExportHeader.h ������
copy OpenXP\�������\SoundExportHeader.h ������

copy ������\include\*.lib ������\Lib
copy ������\include\paintlib\*.lib ������\Lib
copy ������\include\MDFile\*.lib ������\Lib
copy ������\include\xml\*.lib ������\Lib
copy ������\include\zip\*.lib ������\Lib
copy ������\include\zlib\*.lib ������\Lib
copy ������\include\IrrKlang\*.lib ������\Lib

ECHO ɾ�������lib
del  ������\include\*.lib /f /s /q /a

ECHO �������п�
copy OpenXP\bin\*.dll ���п�

ECHO ========================================================== 
ECHO �������� ��������˳�....
rem pause