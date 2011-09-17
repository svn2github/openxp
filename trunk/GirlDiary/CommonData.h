#ifndef __COMMONDATA__H__
#define __COMMONDATA__H__
#pragma once

#define WM_DHITTRLL					WM_USER+1121

#define EPK_NAME					"\\bak\\data.epk"	
#define EPK_PW						"liujia"	

#define EPK_APPNAME_FILE			"appname.txt"

#define EPK_BAKCIMAGE_PATH			"bkimage/"
#define EPK_BACKIMAGE_MAX			20
#define EPK_ERROR_IMAGE_CODE		3
const TCHAR g_szBackImage[EPK_BACKIMAGE_MAX][15] ={"�ͷ�Ģ��.jpg",
													"��������.jpg",
													"��������.jpg",
													"��ɫ����.jpg",
													"������ͩ.jpg",
													"�ò����.jpg",
													"��Өѩ��.jpg",
													"����ʥ��.jpg",
													"��ɫ�ǿ�.jpg",
													"ħ�����.jpg",
													"ħ��ˮ��.jpg",
													"�ϹϾ���.jpg",
													"���ԭҰ.jpg",
													"ʢ�ĺ�̲.jpg",
													"��͸����.jpg",
													"������԰.jpg",
													"ܰ���¾�.jpg",
													"�Ǽʱ���.jpg",
													"ѩ�ɵ���.jpg",
													"��������.jpg"};

#define EPK_TIPIMAGE_PATH			"tip/"
#define MAX_TIP_WETHER				18
const TCHAR g_szWether[MAX_TIP_WETHER][16] = {"��������",
											 "����ϰϰ",	
											 "��ů����",
											 "�̿հ���",
											 "�������",
											 "��������",
											 "������",
											 "��������",
											 "��������",
											 "�����ܲ�",
											 "ëëϸ��",
											 "����ϸ��",
											 "������ע",
											 "ϸѩ��ɳ",
											 "ƮƮ��ѩ",
											 "��ë��ѩ",
											 "ѩ�ϼ�˪",
											 "�������"
											};
#endif