#ifndef _WEB_FILE_UPLOAD_H
#define _WEB_FILE_UPLOAD_H

#include <Arduino.h>

extern String webpage;
extern ESP8266WebServer server;

void HomePage(void);
void File_Download(void);
void FS_file_download(String filename);
void File_Upload(void);
void handleFileUpload(void);
void SendResponseOK(void);

void SendHTML_Header();
void SendHTML_Content();
void SendHTML_Stop();
void SelectInput(String heading1, String command, String arg_calling_name);
void ReportSDNotPresent();
void ReportFileNotPresent(String target);
void ReportCouldNotCreateFile(String target);
String file_size(int bytes);
String getContentType(String filename);

#endif