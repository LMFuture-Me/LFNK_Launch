#pragma warning(disable : 4996)
#pragma warning(disable : 26812)
#pragma warning(disable : 4101)
#pragma warning(disable : 4172)
#include "Header.h"

int checkMD5() {
  if (_access("x64_main.exe", 0) == -1) {
    return -1;
  }
  //如果不符合MD5
  char *MD5 = fileMD5("x64_main.exe");
  char *Web = getWeb(
      "https://raw.staticdn.net/LMFuture-Me/lf_cdn/master/"
      "md5_64.tmp",
      32);
  int st = strcmp(MD5, Web);
  if (st == 0)
    return 1;
  else
    return 0;
}

void Download() {
  puts("Download Function");
  remove("x64_main.exe");
  DOWNLOAD_FILE(
      "https://raw.staticdn.net/LMFuture-Me/lf_cdn/master/x64_main.exe",
      "x64_main.exe");
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
    system("x64_main.exe");
    exit(0);
  }
  CheckUpdate();
  int Status = system("x64_main.exe");
  if (Status != 0) printf("\n[DEBUG] EXITED WITH SOMETHING BAD %d\n", Status);
  return 0;
}