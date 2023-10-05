#pragma once
#include <vector>
#include <string>
struct InputField {
    const char* label;
    const char* hint;
    char* buffer;
    int bufferSize;
    ImGuiInputTextFlags flags;
};

struct PartData {
	int current_id = -1;
	std::vector<std::string> data;
	bool retreived;
};


struct PartQualityData {
	bool selections[20] = {};
	std::vector<std::string> data;
	std::vector<int> order;
	std::string desc = "";
	bool retreived = false;
};

struct Part {
	PartData brand;
	PartData model;
	PartData category;
	PartData color;
	PartQualityData quality;

};
