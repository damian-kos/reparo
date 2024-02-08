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
    // Draw the text on the A4 image using custom font, handling new lines
    PutTextMultiline(a4Image, field.text, cv::Point(field.offset.x, field.offset.y), field.font_size, cv::Scalar(0, 0, 0), ft2);
    std::cout << "Font size on image creation: " << field.font_size << std::endl;
  }
  cv::Mat extraImg = cv::imread("sloth.png"); // Load the extra image
  if (!extraImg.empty()) {
    // Resize the extra image to 60x60 pixels
    cv::Size newSize(logo->size.x, logo->size.y); // New size: 60x60 pixels
    cv::resize(extraImg, extraImg, newSize);

    // Now that extraImg is resized, calculate the center position for extraImg
    //int centerX = (width - extraImg.cols) / 2; // Calculate the X position to center extraImg
    //int posY = logo->offset.y; // Y position is 100 pixels down from the top

    // Define the region of interest on the main image for placing the extra image
    cv::Rect roi(cv::Point(logo->offset.x, logo->offset.y), extraImg.size());

    // Create a new ROI on the main image for the extra image
    cv::Mat destinationROI = a4Image(roi);

    // Copy extraImg to the specified region on a4Image
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
