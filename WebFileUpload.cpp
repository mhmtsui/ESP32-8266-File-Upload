/*  Version 1
 *  
 *  ESP32/ESP8266 example of downloading and uploading a file from or to the ESP device's Filing System
 *  
 This software, the ideas and concepts is Copyright (c) David Bird 2018. All rights to this software are reserved.
 
 Any redistribution or reproduction of any part or all of the contents in any form is prohibited other than the following:
 1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
 2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.
 3. You may not, except with my express written permission, distribute or commercially exploit the content.
 4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.

 The above copyright ('as annotated') notice and this permission notice shall be included in all copies or substantial portions of the Software and where the
 software use is visible to an end-user.
 
 THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT. FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY 
 OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 See more at http://www.dsbird.org.uk
 *
*/
#include "../Ethernet/eethernet.h"
#include "../ESP8266WebServer/src/ESP8266WebServer.h"
#include "../SPIFFS/src/FS.h"
#include "WebFileUpload.h"
#include "CSS.h"


String webpage = "";
// All supporting functions from here...
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HomePage(void){
  SendHTML_Header();
  webpage += F("<a href='/download'><button>Download</button></a>");
  webpage += F("<a href='/upload'><button>Upload</button></a>");
  append_page_footer(webpage);
  SendHTML_Content();
  SendHTML_Stop(); // Stop is needed because no content length was sent
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void File_Download(void){ // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments
//   if (server.args() > 0 ) { // Arguments were received
//     if (server.hasArg("download")) SD_file_download(server.arg(0));
//   }
//   else SelectInput(server, "Enter filename to download","download","download");
// }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void SD_file_download(String filename){
//   if (SD_present) { 
//     File download = SD.open("/"+filename);
//     if (download) {
//       server.sendHeader("Content-Type", "text/text");
//       server.sendHeader("Content-Disposition", "attachment; filename="+filename);
//       server.sendHeader("Connection", "close");
//       server.streamFile(download, "application/octet-stream");
//       download.close();
//     } else ReportFileNotPresent("download"); 
//   } else ReportSDNotPresent(server);
// }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void File_Upload(void){
//   append_page_header(webpage);
//   webpage += F("<h3>Select File to Upload</h3>"); 
//   webpage += F("<FORM action='/fupload' method='post' enctype='multipart/form-data'>");
//   webpage += F("<input class='buttons' style='width:40%' type='file' name='fupload' id = 'fupload' value=''><br>");
//   webpage += F("<br><button class='buttons' style='width:10%' type='submit'>Upload File</button><br>");
//   webpage += F("<a href='/'>[Back]</a><br><br>");
//   append_page_footer(webpage);
//   server.send(200, "text/html",webpage);
// }
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File UploadFile; 
// void handleFileUpload(void){ // upload a new file to the Filing system
//   HTTPUpload& uploadfile = server.upload(); // See https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/srcv
//                                             // For further information on 'status' structure, there are other reasons such as a failed transfer that could be used
//   if(uploadfile.status == UPLOAD_FILE_START)
//   {
//     String filename = uploadfile.filename;
//     if(!filename.startsWith("/")) filename = "/"+filename;
//     Serial.print("Upload File Name: "); Serial.println(filename);
//     SD.remove(filename);                         // Remove a previous version, otherwise data is appended the file again
//     UploadFile = SD.open(filename, FILE_WRITE);  // Open the file for writing in SPIFFS (create it, if doesn't exist)
//     filename = String();
//   }
//   else if (uploadfile.status == UPLOAD_FILE_WRITE)
//   {
//     if(UploadFile) UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
//   } 
//   else if (uploadfile.status == UPLOAD_FILE_END)
//   {
//     if(UploadFile)          // If the file was successfully created
//     {                                    
//       UploadFile.close();   // Close the file again
//       Serial.print("Upload Size: "); Serial.println(uploadfile.totalSize);
//       webpage = "";
//       append_page_header(webpage);
//       webpage += F("<h3>File was successfully uploaded</h3>"); 
//       webpage += F("<h2>Uploaded File Name: "); webpage += uploadfile.filename+"</h2>";
//       webpage += F("<h2>File Size: "); webpage += file_size(uploadfile.totalSize) + "</h2><br>"; 
//       append_page_footer(webpage);
//       server.send(200,"text/html",webpage);
//     } 
//     else
//     {
//       ReportCouldNotCreateFile("upload");
//     }
//   }
// }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SendHTML_Header(){
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate"); 
  server.sendHeader("Pragma", "no-cache"); 
  server.sendHeader("Expires", "-1"); 
  server.setContentLength(CONTENT_LENGTH_UNKNOWN); 
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves. 
  append_page_header(webpage);
  server.sendContent(webpage);
  webpage = "";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SendHTML_Content(){
  server.sendContent(webpage);
  webpage = "";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SendHTML_Stop(){
  server.sendContent("");
  server.client().stop(); // Stop is needed because no content length was sent
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SelectInput(String heading1, String command, String arg_calling_name){
  SendHTML_Header();
  webpage += F("<h3>"); webpage += heading1 + "</h3>"; 
  webpage += F("<FORM action='/"); webpage += command + "' method='post'>"; // Must match the calling argument e.g. '/chart' calls '/chart' after selection but with arguments!
  webpage += F("<input type='text' name='"); webpage += arg_calling_name; webpage += F("' value=''><br>");
  webpage += F("<type='submit' name='"); webpage += arg_calling_name; webpage += F("' value=''><br>");
  webpage += F("<a href='/'>[Back]</a>");
  append_page_footer(webpage);
  SendHTML_Content();
  SendHTML_Stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReportSDNotPresent(){
  SendHTML_Header();
  webpage += F("<h3>No SD Card present</h3>"); 
  webpage += F("<a href='/'>[Back]</a><br><br>");
  append_page_footer(webpage);
  SendHTML_Content();
  SendHTML_Stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReportFileNotPresent(String target){
  SendHTML_Header();
  webpage += F("<h3>File does not exist</h3>"); 
  webpage += F("<a href='/"); webpage += target + "'>[Back]</a><br><br>";
  append_page_footer(webpage);
  SendHTML_Content();
  SendHTML_Stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReportCouldNotCreateFile(String target){
  SendHTML_Header();
  webpage += F("<h3>Could Not Create Uploaded File (write-protected?)</h3>"); 
  webpage += F("<a href='/"); webpage += target + "'>[Back]</a><br><br>";
  append_page_footer(webpage);
  SendHTML_Content();
  SendHTML_Stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
String file_size(int bytes){
  String fsize = "";
  if (bytes < 1024)                 fsize = String(bytes)+" B";
  else if(bytes < (1024*1024))      fsize = String(bytes/1024.0,3)+" KB";
  else if(bytes < (1024*1024*1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
  else                              fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
  return fsize;
}
