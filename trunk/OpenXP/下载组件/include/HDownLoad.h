#ifndef __HDOWNLOAD__H__
#define __HDOWNLOAD__H__
#pragma once

//参数：   hWndOwn -- 拥有窗口
//返回值： -1:重复初始化 1:初始化成功
extern "C" int HDOWNLOAD_API HDownloaderInit(HWND hWndOwn);// 主窗口句柄

//参数：   
//返回值： 
extern "C" void HDOWNLOAD_API HDownloaderEnd();

//参数：   
//返回值： -1:未初始化
extern "C" int HDOWNLOAD_API HDownloaderGetTaskNum();

//参数：  pURL -- 网络文件网址  pSavePath -- 保存路径  pSaveFileName -- 保存文件名  nThreadNum -- 线程数量
//返回值：-1:未初始化 n:新加入的任务的编号
extern "C" int HDOWNLOAD_API HDownloaderAddTask(char *pURL, char *pSavePath, char *pSaveFileName, int nThreadNum);

//参数：  nIndex -- 任务的索引
//返回值：-1:未初始化或任务编号超出范围
extern "C" int HDOWNLOAD_API HDownloaderSelTask(int nIndex); 

//参数：
//返回值：-1:未初始化或无任务
//作用：  移除选中的任务
extern "C" int HDOWNLOAD_API HDownloaderRemoveTask();		 

//参数：  nIndex -- 任务索引  nFileSize -- 网络文件大小 nDownloadedSize -- 下载的大小
//        nDownloadedSize_ThisTimes -- 当次下载的下载(与nDownloadedSize不同，因为有可能断点之后重连的，所以上一次下载的大小不在当次计算之内)        
//返回值：-1:未初始化或任务编号超出范围
extern "C" int HDOWNLOAD_API HDownloaderQueryState(int nIndex,int &nFileSize,int &nDownloadedSize,int &nDownloadedSize_ThisTimes,int &nSpendTime); 

#endif