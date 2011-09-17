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
const TCHAR g_szBackImage[EPK_BACKIMAGE_MAX][15] ={"缤纷蘑菇.jpg",
													"穿云紫藤.jpg",
													"凡间玉兔.jpg",
													"粉色甜蜜.jpg",
													"虹下梧桐.jpg",
													"幻彩舞带.jpg",
													"晶莹雪灵.jpg",
													"快乐圣诞.jpg",
													"栗色星空.jpg",
													"魔堡鬼城.jpg",
													"魔幻水晶.jpg",
													"南瓜精灵.jpg",
													"青翠原野.jpg",
													"盛夏海滩.jpg",
													"剔透心心.jpg",
													"天外桃园.jpg",
													"馨雅温居.jpg",
													"星际宝贝.jpg",
													"雪松弹珠.jpg",
													"艳香郁金.jpg"};

#define EPK_TIPIMAGE_PATH			"tip/"
#define MAX_TIP_WETHER				18
const TCHAR g_szWether[MAX_TIP_WETHER][16] = {"阳光明媚",
											 "凉风习习",	
											 "春暖花开",
											 "碧空白云",
											 "晴空万里",
											 "烈日炎炎",
											 "狂风大作",
											 "电闪雷鸣",
											 "天气阴沉",
											 "乌云密布",
											 "毛毛细雨",
											 "淋淋细雨",
											 "大雨如注",
											 "细雪如沙",
											 "飘飘白雪",
											 "鹅毛大雪",
											 "雪上加霜",
											 "寒冷干燥"
											};
#endif