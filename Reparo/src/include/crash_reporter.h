#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>
// Include necessary Crashpad headers
#include "client/crashpad_client.h"
#include "client/crash_report_database.h"
#include "client/settings.h"
#include "base/files/file_path.h"
#include "imgui.h"


#include <iostream>

class CrashReporter {
public:
  static bool InitializeCrashpad();
  static void CrashOnVec();

};