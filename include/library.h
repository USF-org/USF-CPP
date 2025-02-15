#ifndef LIBRARY_H
#define LIBRARY_H

#include "../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1/iostream"
#include "../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1/fstream"
#include "../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1/string"
#include "../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1/vector"
#include "../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1/map"

#include "json.hpp"

using json = nlohmann::json;

class USF {
public:
    int version;
    std::map<std::string, std::map<std::string, std::string>> subjects;  // simplified_name, teacher, room
    std::vector<std::pair<std::string, std::string>> periods;            // start_time, end_time
    std::vector<std::vector<std::string>> timetable;                     // day, week_type, subject, period

    // 构造函数
    USF();

    // 从文件加载USF数据
    bool load_from_file(const std::string& filename);

    // 保存USF数据到文件
    void save_to_file(const std::string& filename);

    // 添加学科
    void add_subject(const std::string& name, const std::map<std::string, std::string>& details);

    // 添加课程信息
    void add_schedule_entry(int day, const std::string& week_type, const std::string& subject, int period);

    // 校验文件数据
    bool validate();

private:
    // 校验单个字段是否存在
    template <typename T>
    bool check_field(const json& j, const std::string& field, T& result);

    // 校验数据格式
    bool validate_periods();
    bool validate_timetable();
};

#endif // LIBRARY_H