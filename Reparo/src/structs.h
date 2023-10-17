#pragma once

#include "imgui.h"
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
	std::string name = "";
	int IDinDB;
	bool retreived = false;
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

struct Customer {
	std::string name;
	std::string surname;
	std::string email;
	std::string phone_number;
};

struct Repair {
	PartData model;
	PartData category;
	PartData color;
	PartData state;
	std::string note = "";
	std::string note_hidden = "";
	double price;
	Customer customer;
};

struct PopupInput {
	char input[128] = { "" };
	bool is_input_enter_pressed;
	int previous_len;
	bool is_input_text_active;
	bool is_input_text_activated;
};