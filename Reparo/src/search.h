#pragma once
// search.h

#include "imgui.h"
#include "edit_customer.h"
#include "sql_queries.h"
#include "imgui_helper.h"



class Search {
public:
    bool recently_populated;

public:
    void SearchField(const char* searchQuery);
    void ForAdd(std::vector<InputField>& fields, int label_int, SearchFlags = 0);
    SQLQuery sqlSearch;
    bool searchResultsBox = false;
    bool SearchModel(PopupInput& popup, std::vector<std::string>& vector);
    void PopupModels(PopupInput& input, PartData& attribute, const char* label);
    
};
