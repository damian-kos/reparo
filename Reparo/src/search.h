#pragma once
// search.h

#include "imgui.h"
#include "edit_customer.h"
#include "sql_queries.h"
#include "imgui_helper.h"


class Search {
public:
    void ForEdit();
    void SearchField(const char* searchQuery);
    void ForAdd(std::vector<InputField>& fields);
};

