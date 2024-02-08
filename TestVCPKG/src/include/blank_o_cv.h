#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/freetype.hpp>
#include <vector>
#include <iostream>
#include "text_field.h"

struct PutTextScales {
public:
  static constexpr float ASCENT_SCALE = 0.84f;

};

class ImageEditor {
public:
  static void CreateA4(std::vector<TextField> &text_fields_vector, Logo* logo);
  

};