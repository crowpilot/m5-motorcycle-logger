#ifndef httpAP_h
#define httpAP_h
#include<M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPI.h>
#include <SD.h>

void initServer();
void refreshServer();


void returnOK();
void returnFail(String msg);
bool loadFromSdCard(String path);
void handleFileUpload();
void deleteRecursive(String path);
void handleDelete();
void handleCreate();
void printDirectory();
void handleNotFound();

#endif
