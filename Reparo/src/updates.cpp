#include "updates.h"
#include <iostream>
#include <filesystem>
#include <direct.h>


#include <windows.h>


void Updater::ModalForUpdate() {

  if (update_available) {
    ImGui::OpenPopup("Update Available");
    update_available = false;
  }
  if (ImGui::BeginPopupModal("Update Available", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text(initial_text.c_str());

    if (!update_downloaded) {
      if (ImGui::Button("Yes")) {
        Updater::update_confirm = true;

#ifdef _DEBUG
        update_downloaded = !update_downloaded;
#endif
#ifndef _DEBUG
        update_downloaded = ProcessUpdate();
#endif
      }
      if (ImGui::Button("Cancel")) {
        Updater::update_confirm = false;
        ImGui::CloseCurrentPopup();
      }
    }
    else {
      initial_text = "Updated downloaded successfuly.";
      if (ImGui::Button("Restart")) 
        ImGui::CloseCurrentPopup();
      
    }
    ImGui::EndPopup();
  }
}

bool Updater::ProcessUpdate() {
  if (update_confirm) {
    std::string link = GetDetailsFromAPI(1);
    std::string changes_file = "tracked_changes.txt";

    if (DwnChangesTxt(link, changes_file)) {
      std::cout << "Downloaded tracked_changes.txt successfully." << std::endl;
    }
    else {
      std::cerr << "Failed to download tracked_changes.txt." << std::endl;
      return false;
    }
    std::cout << "Processing update  " << available_version << std::endl;

    DownloadZipball(available_version);
    Unzipper(available_version);
    DeleteDwnZip();
    ApplyReadChanges("tracked_changes.txt", available_version);
    update_confirm = false;
    return true;
  }
  else {
    return false;
  }
}

size_t Updater::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
  size_t total_size = size * nmemb;
  data->append(static_cast<char*>(contents), total_size);
  return total_size;
}

std::string Updater::GetDetailsFromAPI(int case_call) {
  using json = nlohmann::json;

  CURL* curl = curl_easy_init();
  if (curl) {
    std::string url = "https://api.github.com/repos/damian-kos/reparo_updates/releases/latest";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set User-Agent header:
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "reparo_updates");

    // Set up callback function to handle received data
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform the HTTP request
    curl_easy_perform(curl);

    // Check HTTP response code
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    std::cout << "HTTP CODE GetDetailsFromAPI: " << httpCode << std::endl;

    // Clean up
    curl_easy_cleanup(curl);

    if (httpCode == 403) {
      std::cerr << "Access denied. Check your API key or authentication." << std::endl;
      return "";
    }
    else if (httpCode == 429) {
      std::cerr << "API rate limit exceeded. Please wait and try again later." << std::endl;
      return "";
    }
    else if (httpCode != 200) {
      std::cerr << "HTTP request failed with code " << httpCode << std::endl;
      return "";
    }

    try {
      json jsonData = json::parse(response);
      std::string result;
      // Access the "assets" array and get the "browser_download_url" from the first element
      if (case_call == 0) {
        result = jsonData["name"];
      }
      else if(case_call == 1) {
        result = jsonData["assets"][0]["browser_download_url"];
      }

      // Return the extracted data or handle it as needed
      return result;
    }
    catch (const std::exception& e) {
      // Handle JSON parsing exception
      std::cerr << "JSON parsing error in GetDetailsFromAPI: " << e.what() << std::endl;
      return "";
    }
  }
  return "";
}

bool Updater::DwnChangesTxt(const std::string& url, const std::string& outputFile) {
  CURL* curl = curl_easy_init();
  if (curl) {
    FILE* file = fopen(outputFile.c_str(), "wb");
    if (!file) {
      std::cerr << "Failed to open file for writing: " << outputFile << std::endl;
      return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "reparo_updates");
    curl_easy_setopt(curl, CURLOPT_HEADER, false);

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    CURLcode res = curl_easy_perform(curl);
    fclose(file);

    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      return false;
    }

    curl_easy_cleanup(curl);
    return true;
  }

  return false;
}

void Updater::ApplyReadChanges(std::string file_name, std::string latest_ver) {

  std::ifstream file(file_name);

  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      // Extract the filename using std::find_last_of to find the last '/'
      size_t lastSlashPos = line.find_last_of("/\\");
      std::string filename = line.substr(lastSlashPos + 1);

      // Remove non-printable characters from the filename
      filename.erase(std::remove_if(filename.begin(), filename.end(),
        [](unsigned char c) { return !std::isprint(c); }),
        filename.end());
      line.erase(std::remove_if(line.begin(), line.end(),
        [](unsigned char c) { return !std::isprint(c); }),
        line.end());

      std::cout << "\n"  << "Line: " << line << std::endl;

      if (filename.ends_with(".exe")) {
        std::cout << "This is an executable file." << std::endl;
        MoveOldVerFile(line);
      }
      else if (filename.ends_with(".dll")) {
        std::cout << "This is an DLL file." << std::endl;
        MoveOldVerFile(line);
      }
        MoveFiles(line, latest_ver);
    }
    file.close();
  }
  else {
    std::cerr << "Error opening file: " << file_name << std::endl;
  }
}

size_t Updater::write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
  return written;
}

void Updater::DownloadZipball(std::string latest_ver) {
  std::string url = "https://api.github.com/repos/damian-kos/reparo_updates/zipball/" + latest_ver;
  CURL* curl_handle;
  CURLcode res;
  static const char* page_filename = "bin/data.zip";
  FILE* pagefile;

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();
  /* set URL to get here */
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "reparo_updates");
  curl_easy_setopt(curl_handle, CURLOPT_HEADER, false);
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

  /* disable progress meter, set to 0L to enable and disable debug output */
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
  /* open the file */
  pagefile = fopen(page_filename, "wb");
  if (pagefile) {

    /* write the page body to this file handle */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

    res = curl_easy_perform(curl_handle);
    /* close the header file */
    fclose(pagefile);
  }

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  curl_global_cleanup();
  std::cout << "Downloaded " << std::endl;
}


std::string Updater::ReplaceFirstPart(const std::string& original, const std::string& replacement) {
  size_t end_pos = original.find('/');

  if (end_pos != std::string::npos) {
    return replacement + original.substr(end_pos);
  }
  else {
    // Handle the case where the path format is unexpected
    return original;
  }
}

void Updater::Unzipper(std::string latest_ver) {
  const char* zip_file_path = "bin/data.zip";
  const char* extract_path = "bin";

  // Open the ZIP file
  int errorp = 0;
  zip* archive = zip_open(zip_file_path, ZIP_RDONLY, &errorp);
  if (!archive) {
    std::cerr << "Error opening ZIP file: " << zip_strerror(archive) << std::endl;
  }

  // Get the number of entries (files/folders) in the ZIP file
  zip_int64_t num_entries = zip_get_num_entries(archive, 0);
  if (num_entries < 0) {
    std::cerr << "Error getting number of entries: " << zip_strerror(archive) << std::endl;
    zip_close(archive);
  }

  // Create the extraction directory
  if (_mkdir(extract_path) != 0 && errno != EEXIST) {
    std::cerr << "Error creating extraction directory: " << strerror(errno) << std::endl;
    zip_close(archive);
  }

  // Extract each entry in the ZIP file
  for (zip_int64_t i = 0; i < num_entries; ++i) {
    struct zip_stat file_stat;
    if (zip_stat_index(archive, i, 0, &file_stat) == 0) {
      // Construct the path to extract the file to
      std::string changed_str = ReplaceFirstPart(file_stat.name, latest_ver.c_str());
      std::string extract_file_path = std::string(extract_path) + "/" + changed_str;

      // Check if the entry is a directory
      if (file_stat.name[strlen(file_stat.name) - 1] == '/') {
        if (_mkdir(extract_file_path.c_str()) != 0 && errno != EEXIST) {
          std::cerr << "Error creating directory: " << strerror(errno) << std::endl;
          zip_close(archive);
        }
      }
      else {
        // Extract the file
        zip_file* file = zip_fopen_index(archive, i, 0);
        if (file) {
          FILE* out_file = fopen(extract_file_path.c_str(), "wb");
          if (out_file) {
            const int buffer_size = 8192;
            char buffer[buffer_size];
            zip_int64_t bytes_read;
            while ((bytes_read = zip_fread(file, buffer, buffer_size)) > 0) {
              fwrite(buffer, 1, static_cast<size_t>(bytes_read), out_file);
            }
            fclose(out_file);
          }
          else {
            std::cerr << "Error creating output file: " << strerror(errno) << std::endl;
            zip_fclose(file);
            zip_close(archive);

          }
          zip_fclose(file);
        }
        else {
          std::cerr << "Error opening file in ZIP: " << zip_strerror(archive) << std::endl;
          zip_close(archive);

        }
      }
    }
    else {
      std::cerr << "Error getting file stat: " << zip_strerror(archive) << std::endl;
      zip_close(archive);

    }
  }

  // Close the ZIP archive
  zip_close(archive);
  std::cout << "Extraction complete." << std::endl;
}

void Updater::DeleteDwnZip() {
  const char* filename = "bin/data.zip";
  // Attempt to remove the file
  if (std::remove(filename) == 0) {
    printf("File %s deleted successfully.\n", filename);
  }
  else {
    perror("Error deleting file");
  }
}


void Updater::RenameAndMovePrevExe() {
  std::cout << "Executable path (__FILE__): " << __FILE__ << std::endl;

  if (std::rename("Reparo.exe", "Old.exe") == 0) {
    printf("File renamed successfuly.\n");
  }
  else {
    printf("Not renamed.\n");
  }
  // Path to the source file
  std::filesystem::path source_file_path = "Old.exe";
  //// Path to the destination folder
  std::filesystem::path destination_dir_path = "bin/" + std::string(REPARO_VERSION);

  try {
    // Create the destination path by appending the filename to the destination folder path
    std::filesystem::path destinationFilePath = destination_dir_path / source_file_path.filename();
    // Create the destination folder if it doesn't exist
    std::filesystem::create_directories(destination_dir_path);
    // Perform the move operation
    std::filesystem::rename(source_file_path, destinationFilePath);

    std::cout << "File moved successfully!" << std::endl;
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

}

void Updater::MoveFiles(std::string source, std::string latest_ver) {
  // Specify the version folder
  std::string version_dir = "bin";

  // Path to the source file
  std::filesystem::path src_file_path = std::filesystem::path(version_dir) / latest_ver.c_str() / source;
  std::cout << "MoveFiles Source path: " << src_file_path << std::endl;

  // Path to the destination folder (main directory)
  std::filesystem::path dest_dir_path = std::filesystem::current_path();

  try {
    // Create the destination path by appending the filename to the destination folder path
    std::filesystem::path dest_file_path = dest_dir_path / source;
    std::cout << "MoveFiles Destination path: " << dest_file_path << std::endl;

    // Create the destination folder if it doesn't exist
    std::filesystem::create_directories(dest_dir_path);

    // Perform the move operation
    std::filesystem::rename(src_file_path, dest_file_path);

    std::cout << "File moved successfully! \n" << std::endl;
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n" << std::endl;
  }
}

void Updater::MoveOldVerFile(const std::string& file) {
  std::filesystem::path src_file = std::filesystem::current_path() / file;
  std::cout << "Source path of file to change name: "  << src_file << std::endl;

  std::filesystem::path old_ver_dest_dit = "bin/" + std::string(REPARO_VERSION);
  try {
    std::filesystem::create_directories(old_ver_dest_dit);

    std::filesystem::path new_file_dest = old_ver_dest_dit / src_file.filename();

    std::filesystem::rename(src_file, new_file_dest);
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}