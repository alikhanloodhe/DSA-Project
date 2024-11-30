#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip> // Required for put_time]=
#include <string>
using namespace std;

class Time{
    
    public:
    Time(){ }
    ~Time(){ }
    string get_current_time() {
    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Convert to string format (12-hour format with AM/PM)
    std::tm* tm_now = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(tm_now, "%I:%M:%S %p");  // 12-hour format with AM/PM
    return oss.str();
}
string get_current_date() {
    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Convert to string format (YYYY-MM-DD)
    std::tm* tm_now = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(tm_now, "%Y-%m-%d");
    return oss.str();
}
};