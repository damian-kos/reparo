#pragma once

#include <iostream>

#include "imgui.h"
#include "imgui_viewer.h"
#include "database.h"

class Finances {
public:
  Finances();
  ~Finances();

public:
  void Render();
};