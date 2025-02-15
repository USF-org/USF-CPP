#include "library.h"

USF::USF() : version(1) {}

bool USF::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << "\n";
        return false;
    }

    try {
        json j;
        file >> j;

        // 检查是否有必需的字段而不是Zako篡改了的文件
        if (!check_field(j, "version", version) || !check_field(j, "subjects", subjects) ||
            !check_field(j, "periods", periods) || !check_field(j, "timetable", timetable)) {
            std::cerr << "Missing required fields in the JSON data\n";
            return false;
        }

        // 校验数据格式
        if (!validate()) {
            std::cerr << "Invalid data format in USF file\n";
            return false;
        }

        file.close();
    } catch (const json::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << "\n";
        return false;
    }

    return true;
}

void USF::save_to_file(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << "\n";
        return;
    }

    json j;
    j["version"] = version;
    j["subjects"] = subjects;
    j["periods"] = periods;
    j["timetable"] = timetable;

    file << j.dump(4);  // Pretty print with 4 spaces indent
    file.close();
}

void USF::add_subject(const std::string& name, const std::map<std::string, std::string>& details) {
    subjects[name] = details;
}

void USF::add_schedule_entry(int day, const std::string& week_type, const std::string& subject, int period) {
    timetable.push_back({ std::to_string(day), week_type, subject, std::to_string(period) });
}

bool USF::validate() {
    // 验证 periods
    if (!validate_periods()) {
        std::cerr << "Invalid periods data\n";
        return false;
    }

    // 验证 timetable
    if (!validate_timetable()) {
        std::cerr << "Invalid timetable data\n";
        return false;
    }

    return true;
}

template <typename T>
bool USF::check_field(const json& j, const std::string& field, T& result) {
    if (j.find(field) == j.end()) {
        std::cerr << "Missing field: " << field << "\n";
        return false;
    }

    try {
        result = j[field].get<T>();
    } catch (const json::exception& e) {
        std::cerr << "Error reading field " << field << ": " << e.what() << "\n";
        return false;
    }

    return true;
}

bool USF::validate_periods() {
    for (const auto& period : periods) {
        if (period.first.empty() || period.second.empty()) {
            std::cerr << "Invalid period entry: both start and end times must be provided\n";
            return false;
        }
    }
    return true;
}

bool USF::validate_timetable() {
    for (const auto& entry : timetable) {
        if (entry.size() != 4) {
            std::cerr << "Invalid timetable entry, should have 4 items (day, week_type, subject, period)\n";
            return false;
        }

        int day = std::stoi(entry[0]);
        if (day < 1 || day > 7) {
            std::cerr << "Invalid day in timetable: should be between 1 and 7\n";
            return false;
        }

        const std::string& week_type = entry[1];
        if (week_type != "all" && week_type != "even" && week_type != "odd") {
            std::cerr << "Invalid week type in timetable: should be 'all', 'even', or 'odd'\n";
            return false;
        }

        if (subjects.find(entry[2]) == subjects.end()) {
            std::cerr << "Subject " << entry[2] << " not found in subjects\n";
            return false;
        }

        int period = std::stoi(entry[3]);
        if (period < 1 || period > periods.size()) {
            std::cerr << "Invalid period in timetable\n";
            return false;
        }
    }

    return true;
}