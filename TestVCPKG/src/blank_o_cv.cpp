#include "blank_o_cv.h"

void PutTextMultiline(cv::Mat& image, const std::string& text, const cv::Point& org, int fontHeight, const cv::Scalar& color, cv::Ptr<cv::freetype::FreeType2>& ft2) {
  std::istringstream stream(text);
  std::string line;
  int lineHeight = fontHeight; // Adjust line height as needed
  int estimatedAscent = fontHeight * PutTextScales::ASCENT_SCALE; // This is an approximation and may need adjustment
  int currentY = org.y + estimatedAscent; // Start from adjusted Y position to simulate top-left corner as starting point
  std::cout << "line height: " << lineHeight << " estimated ascent: " << estimatedAscent << " current Y ool: " << currentY << " font height: " << fontHeight << std::endl;
  while (std::getline(stream, line)) {
    cv::Point lineOrg(org.x, currentY);
    ft2->putText(image, line, lineOrg, fontHeight*0.9, color, -1, cv::LINE_AA, true);
    currentY += lineHeight; // Move to the next line position correctly
    std::cout << "current Y ial: " << currentY << std::endl;
  }
}

void drawRoundedRectangle(cv::Mat& img, cv::Point topLeft, cv::Point bottomRight, int cornerRadius, const cv::Scalar& color, const cv::Scalar& color_fill, int thickness) {
  // Ensure thickness is positive or filled
  thickness = thickness > 0 ? thickness : -1;

  // Calculate corner points
  int x1 = topLeft.x;
  int y1 = topLeft.y;
  int x2 = bottomRight.x;
  int y2 = bottomRight.y;

  // Draw the four corners: top left, top right, bottom right, bottom left
  cv::ellipse(img, cv::Point(x1 + cornerRadius, y1 + cornerRadius), cv::Size(cornerRadius, cornerRadius), 180.0, 0, 90, color, thickness);
  cv::ellipse(img, cv::Point(x2 - cornerRadius, y1 + cornerRadius), cv::Size(cornerRadius, cornerRadius), 270.0, 0, 90, color, thickness);
  cv::ellipse(img, cv::Point(x2 - cornerRadius, y2 - cornerRadius), cv::Size(cornerRadius, cornerRadius), 0.0, 0, 90, color, thickness);
  cv::ellipse(img, cv::Point(x1 + cornerRadius, y2 - cornerRadius), cv::Size(cornerRadius, cornerRadius), 90.0, 0, 90, color, thickness);

  // Draw the four sides: top, right, bottom, left
  cv::line(img, cv::Point(x1 + cornerRadius, y1), cv::Point(x2 - cornerRadius, y1), color, thickness);
  cv::line(img, cv::Point(x2, y1 + cornerRadius), cv::Point(x2, y2 - cornerRadius), color, thickness);
  cv::line(img, cv::Point(x1 + cornerRadius, y2), cv::Point(x2 - cornerRadius, y2), color, thickness);
  cv::line(img, cv::Point(x1, y1 + cornerRadius), cv::Point(x1, y2 - cornerRadius), color, thickness);
  // choose arbitrary starting point for fill => Top left plus 10,10
  cv::Point fillFrom(topLeft.x + 10, topLeft.y + 10);
  // You may want to use `lineColor` instead of `fillColor`
  cv::floodFill(img, fillFrom, color_fill);
}


void ImageEditor::CreateA4(std::vector<TextField>& text_fields_vector, Logo* logo) {

  // Convert A4 size from inches to pixels (corrected conversion using PPI)
  int width = static_cast<int>(210 * 2.5);
  int height = static_cast<int>(297 * 2.5);

  // Create a blank white A4 image (3 channels for RGB, with white color)
  cv::Mat a4Image(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

  // Path to your custom font
  std::string fontPath = "c:\\Windows\\Fonts\\arial.ttf";

  // Create a FreeType2 instance
  cv::Ptr<cv::freetype::FreeType2> ft2 = cv::freetype::createFreeType2();

  // Load your custom font
  ft2->loadFontData(fontPath, 0);

  // Text settings for custom font
  for (const auto& field : text_fields_vector) {
  // Draw the rectangle on the image

    cv::Scalar rectColor(0, 0, 0); // Black border
    cv::Scalar rectColor2(255, 175, 0); // Black border

    int thickness = 1; // Pixel
    int cornerRadius = 5; // Radius of the rounded corners
    cv::Scalar color_fill(255, 175, 0);

    drawRoundedRectangle(a4Image, cv::Point(field.offset.x-field.font_size/3, field.offset.y-field.font_size/4),
                         cv::Point(field.offset.x + field.size.x, field.offset.y + field.size.y),
                         cornerRadius, rectColor, color_fill, thickness);
    cv::Scalar color_fill1(255, 120, 0);

    drawRoundedRectangle(a4Image, cv::Point(field.offset.x +field.size.x + 40 , field.offset.y - field.font_size / 4),
                         cv::Point(field.offset.x + field.size.x * 2, field.offset.y  + field.size.y), 
                         cornerRadius, rectColor, color_fill1, thickness);
      // Draw the text on the A4 image using custom font, handling new lines
    PutTextMultiline(a4Image, field.text, cv::Point(field.offset.x, field.offset.y), field.font_size, cv::Scalar(0, 0, 0), ft2);
    std::cout << "Font size on image creation: " << field.font_size << std::endl;

  }
  cv::Mat extraImg = cv::imread("sloth.png"); // Load the extra image
  if (!extraImg.empty()) {
    // Resize the extra image to 60x60 pixels
    cv::Size newSize(logo->size.x, logo->size.y); // New size: 60x60 pixels
    cv::resize(extraImg, extraImg, newSize);
    cv::Rect roi(cv::Point(logo->offset.x, logo->offset.y), extraImg.size());
    cv::Mat destinationROI = a4Image(roi);
    extraImg.copyTo(destinationROI);
  }
  else {
    std::cerr << "Failed to load extra image." << std::endl;
  }

  // Save the image or display
  cv::imwrite("a4_hello_world.jpg", a4Image); // Save the image
  cv::imshow("A4 Image with Custom Font", a4Image); // Display the image
  cv::waitKey(0); // Wait for a key press to close the image window
}
