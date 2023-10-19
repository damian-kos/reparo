#pragma once

#include "imgui.h"
#include <vector>
#include <string>

typedef int CustomerInputFlags;
typedef int SearchFlags;
typedef int CustomerSubmissionFlags;

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
	int id;
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

enum CustomerSubmissionState_ {
	NotSubmitted = -3,
	PhoneNumberIsEmpty = -2,
	WrongQuery = -1,
	AddNewCustomer = 0,
};

enum CustomerSubmissionFlags_ {
	CustomerSubmissionFlags_None = 0,
	CustomerSubmissionFlags_SimpleAdd = 1 << 0,
	CustomerSubmissionFlags_RepairAdd = 1 << 1,
};

enum SearchFlags_ {
	SearchFlags_None = 0,
	SearchFlags_EditCustomer = 1 << 1,
	SearchFlags_CopyToFields = 1 << 2,
};

enum CustomerInputFlags_ {
	CustomerInputFlags_None = 0,
	CustomerInputFlags_SubmitButton = 1 << 0,
	CustomerInputFlags_NameField = 1 << 1,
	CustomerInputFlags_NoSurnameField = 1 << 2,
	CustomerInputFlags_EmailField = 1 << 3,
	CustomerInputFlags_SearchResultsOnPhoneNo = 1 << 4,
};