# ESP32-8266-FIle-Upload

Using HTTP and an HTML interface to download files from an ESP32/ESP8266

Download all files to a sketch folder.Edit the Network tab and add in your SSID and PASSWORD, more if you have them.Choose your IP address, currently it is fixed to 192.168.0.150You can edit the logical name 'fileserver' to your requirements then access the device with http://fileserver.local but only if your browser has mDNS support otherwise use http://192.168.0.150/NOTE: the Directory command is not included in this release, this comes later.To test the download place a known file on the SD-Card for trial purposes.NOTES:The ESP32 is not reliable when using SD Cards, please ensure you know how to connect the SPI bus to the SD-Card if not using an MH-ET Live ESP32 board and a Wemos SD-Card Shield. Although pull-ups are enabled, you may need to add an external 4k7 pull-up too on the MISO line.

