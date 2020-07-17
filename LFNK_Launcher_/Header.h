#pragma once
#pragma warning(disable : 4996)
#pragma warning(disable : 26812)
#pragma warning(disable : 4101)
#pragma warning(disable : 4172)
#pragma comment(lib, "libcurl.lib")
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#include <iostream>

#include "includes/curl.h"

// FILE DOWNLOADER
size_t onDataReceive(void *buffer, size_t size, size_t nmemb, void *user_p) {
  FILE *fp = (FILE *)user_p;
  size_t receivedDataLen;
  receivedDataLen = fwrite(buffer, size, nmemb, fp);
  return receivedDataLen;
}
int progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow,
                      curl_off_t ultotal, curl_off_t ulnow) {
  if (0 != dltotal) {
  }
  return 0;
}
int DOWNLOAD_FILE(const char *URL, const char *FileNameSave) {
  std::string url = URL;
  std::string filename = FileNameSave;
  CURLcode code;
  // 设置运行环境
  code = curl_global_init(CURL_GLOBAL_ALL);
  if (CURLE_OK != code) {
    std::cerr << "init libcurl failed." << std::endl;
    return -1;
  }
  // curl提供两类接口，分别是easy interface和multi interface
  // 想要使用easy interface需要先获取easy hanle
  CURL *easy_handle = curl_easy_init();
  void *clientp = NULL;
  FILE *fp;
  fopen_s(&fp, filename.c_str(), "ab+");
  // 开启进度设置
  curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0);

  // 设置进度回调函数
  curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, progress_callback);

  // 设置传给进度回调函数的指针
  curl_easy_setopt(easy_handle, CURLOPT_XFERINFODATA, clientp);

  // 设置下载地址
  curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());

  // 设置接受数据的回调函数
  curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, &onDataReceive);

  // 设置传给回调函数的参数，这里是个文件指针
  curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, fp);

  // 连接远端站点，执行必要的命令并处理返回结果
  curl_easy_perform(easy_handle);

  curl_easy_cleanup(easy_handle);
  fclose(fp);

  curl_global_cleanup();
  return 0;
}

// getFile
char *getFile(const char *FileName, int MaxCount) {
  FILE *fp;
  if ((fp = fopen(FileName, "r")) == NULL) return NULL;
  char Re[1024];
  fgets(Re, MaxCount, fp);
  fclose(fp);
  return Re;
}

// GETWEB
char *getWeb(const char *URL, int MaxL) {
  char command[1024];
  DOWNLOAD_FILE(URL, "web.tmp");
  char *web = getFile("web.tmp", MaxL);
  remove("web.tmp");
  return web;
}

// system INFO
VOID SafeGetNativeSystemInfo(__out LPSYSTEM_INFO lpSystemInfo) {
  if (NULL == lpSystemInfo) return;
  typedef VOID(WINAPI * LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
  LPFN_GetNativeSystemInfo fnGetNativeSystemInfo =
      (LPFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandle(_T("kernel32")),
                                               "GetNativeSystemInfo");
  ;
  if (NULL != fnGetNativeSystemInfo) {
    fnGetNativeSystemInfo(lpSystemInfo);
  } else {
    GetSystemInfo(lpSystemInfo);
  }
}
int GetSystemBits() {
  SYSTEM_INFO si;
  SafeGetNativeSystemInfo(&si);
  if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
      si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
    return 64;
  }
  return 32;
}

// Get MD5
char *fileMD5(const char *FileName) {
  FILE *fp;
  char Command[128];
  sprintf(Command, "md5.exe -f %s>md5.tmp", FileName);
  system(Command);
  if ((fp = fopen("md5.tmp", "r")) == NULL) return NULL;
  char Re[1024];
  fgets(Re, 32, fp);
  fclose(fp);
  remove("md5.tmp");

  return Re;
}

// NetworkChecker
int CheckIsNetWorking() {
  //创建curl对象
  CURL *curl;
  //创建curlcode对象
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    //设置一个请求文件地址
    curl_easy_setopt(
        curl, CURLOPT_URL,
        "https://raw.staticdn.net/LMFuture-Me/lf_cdn/master/check");
    //执行请求操作,将返回值交给res
    res = curl_easy_perform(curl);
    // res=0的情况表示接收到信息,反之则没有收到信息
    if (res != 0) {
      //没有联网
      return false;
    } else {
      //已经联网
      return true;
    }
    /* 释放资源 */
    curl_easy_cleanup(curl);
  }
  return false;
}