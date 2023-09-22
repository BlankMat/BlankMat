#include "fileHelper.h"

// Returns whether the given file exists
bool FileExists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

// Rounds the value to the given precision
std::string RoundToPrecision(float _val, size_t _precision)
{
    size_t nzero = _precision + 1;
    std::string str = std::to_string(_val);
    auto new_str = std::string(nzero - std::min(nzero, str.length()), '0') + str;
    return new_str;
}

// Help parsing a string by token from:
// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
void ParseStringByDelim(std::vector<std::string>& out, std::string str, std::string delim)
{
    // Make sure out vec is cleared
    out.clear();

    // Parse each part of the string
    size_t spos = 0;
    std::string token;
    std::string tempStr = str;
    while ((spos = tempStr.find(delim)) != std::string::npos) {
        // Parse token
        token = tempStr.substr(0, spos);
        out.push_back(token);
        tempStr.erase(0, spos + delim.length());
    }
    out.push_back(tempStr);
}

// Reads a vector 3 from the given strings, skipping ahead the given offset. Strings must have size >= 4 + offset to be read, and the first element + offset are ignored
glm::vec3 ReadVec3FromStrings(std::vector<std::string>& strings, int offset)
{
    // Avoid errors
    if (strings.size() >= 4 + offset)
        return glm::vec3(std::stof(strings[1 + offset]), std::stof(strings[2 + offset]), std::stof(strings[3 + offset]));
    else
        return glm::vec3();
}


// Prints the given array
void PrintArray(std::string startText, float arr[], int n, int elemsPerLine)
{
    using namespace std;
    cout << startText << endl;
    cout << "Size: " << n << endl;

    cout << fixed << setprecision(3);

    // Print every element in the array
    for (int i = 0; i < n; i++) {
        // Print index on first line
        if (i % elemsPerLine == 0)
            cout << "[" << (int)(i / elemsPerLine) << "] ";

        // Print element
        cout << "(" << arr[i] << ")";

        // Swap rows every elemsPerLine
        if ((i + 1) % elemsPerLine == 0)
            cout << endl;
        // Otherwise separate lines
        else
            cout << ", ";
    }

    // Finish with newlines
    cout << endl << endl;
}

// Prints the given array
void PrintArray(std::string startText, unsigned int arr[], int n, int elemsPerLine)
{
    using namespace std;
    cout << startText << endl;
    cout << "Size: " << n << endl;

    // Print every element in the array
    for (int i = 0; i < n; i++) {
        // Print index on first line
        if (i % elemsPerLine == 0)
            cout << "[" << (int)(i / elemsPerLine) << "] ";

        // Print element
        cout << "(" << arr[i] << ")";

        // Swap rows every elemsPerLine
        if ((i + 1) % elemsPerLine == 0)
            cout << endl;
        // Otherwise separate lines
        else
            cout << ", ";
    }

    // Finish with newlines
    cout << endl << endl;
}