#pragma once
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include <zip.h>

#include "modals.h" // BEWARE 


#define REPARO_VERSION "0.0.9"

class Updater {
public:

  static void ModalForUpdate();

  static bool ProcessUpdate();

  static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data);
  static std::string GetDetailsFromAPI(int case_call);
  static bool DwnChangesTxt(const std::string& url, const std::string& outputFile);
  static void ApplyReadChanges(std::string file_name, std::string latest_ver);
  static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream);
  static void DownloadZipball(std::string latest_ver);
  static std::string ReplaceFirstPart(const std::string& original, const std::string& replacement);
  static void Unzipper(std::string latest_ver);
  static void DeleteDwnZip();
  static void RenameAndMovePrevExe();
  static void MoveFiles(std::string source, std::string latest_ver);
  static void MoveOldVerFile(const std::string& file);

public:
  static inline bool update_confirm = false;
  static inline bool update_available = false;
  static inline bool update_downloaded = false;
  static inline std::string available_version = "";
  static inline std::string initial_text = "There is update avaiable.Do you want to download a new version?";
};