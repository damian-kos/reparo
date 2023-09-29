#pragma once

struct InputField {
    const char* label;
    const char* hint;
    char* buffer;
    int bufferSize;
    ImGuiInputTextFlags flags;
};