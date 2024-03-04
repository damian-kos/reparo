#include "crash_reporter.h"

bool CrashReporter::InitializeCrashpad() {
  base::FilePath::StringType cwd = base::FilePath::kCurrentDirectory;
  base::FilePath handler(L"crashpad_handler.exe");
  base::FilePath database(cwd.append(L"/database"));
  base::FilePath metrics_dir(cwd.append(L"/metrics"));

  crashpad::CrashpadClient client;
  std::map<std::string, std::string> annotations;
  base::FilePath::kCurrentDirectory;

  // Add more detailed annotations
  annotations["version"] = "1.0.0"; // Example: Application version
  annotations["user_id"] = "12345"; // Example: User identifier

  std::vector<std::string> arguments;
  // Disable rate limiting in development
  arguments.push_back("--no-rate-limit");

  // Configure Crashpad with detailed minidumps and custom callbacks

  bool success = client.StartHandler(
    handler,
    database,
    metrics_dir,
    "",
    annotations,
    arguments,
    true, // Start handler at crash
    true  // Start handler on demand
  );

  return success; // Return the success status of starting the handler
}

void CrashReporter::CrashOnVec() {
  if (ImGui::Button("Test on vec")) {
    abort();
  }
}
