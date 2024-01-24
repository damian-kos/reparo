#pragma once
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include <zip.h>

#define REPARO_VERSION "0.0.9"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data);
std::string GetDetailsFromAPI(int case_call);
bool DwnChangesTxt(const std::string& url, const std::string& outputFile);
void ApplyReadChanges(std::string file_name, std::string latest_ver);
void DownloadZipball(std::string latest_ver);
void Unzipper(std::string latest_ver);
void DeleteDwnZip();
void RenameAndMovePrevExe();
void MoveFiles(std::string source, std::string latest_ver);
void MoveOldVerFile(const std::string& file);
