#pragma warning(disable : 4996)
#pragma warning(disable : 26812)
#pragma warning(disable : 4101)
#pragma warning(disable : 4172)
#include "Header.h"

#define SoftwareMD5DownloadAddress "https://raw.staticdn.net/LMFuture-Me/lf_cdn/master/md5_64.tmp"
#define SoftwareExeFileDownloadAddress "https://raw.staticdn.net/LMFuture-Me/lf_cdn/master/x64_main.exe"
#define SoftwareExeFileName "x64_main.exe"

int checkMD5() {
  if (_access(SoftwareExeFileName, 0) == -1) {
    return -1;
  }
  //如果不符合MD5
  char *MD5 = fileMD5(SoftwareExeFileName);
  char *Web = getWeb(SoftwareMD5DownloadAddress, 32);
  int st = strcmp(MD5, Web);
  if (st == 0)
    return 1;
  else
    return 0;
}

void Download() {
  puts("Download Function");
  remove(SoftwareExeFileName);
  DOWNLOAD_FILE(SoftwareExeFileDownloadAddress,
      SoftwareExeFileName);
}

void CheckUpdate() {
  while (checkMD5() != 1) {
    Download();
  }
}

int main(void) {
  system("mode con cols=42 lines=29");
  system("title LiteFish System Detector");
  system("color 70");
  if (!CheckIsNetWorking()) {
    system(SoftwareExeFileName);
    exit(0);
  }
  CheckUpdate();
  int Status = system(SoftwareExeFileName);
  if (Status != 0) printf("\n[DEBUG] EXITED WITH SOMETHING BAD %d\n", Status);
  return 0;
}