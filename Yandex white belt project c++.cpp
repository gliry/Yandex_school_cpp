#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <set>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

class Date {
public:
    int GetYear() const {
        return year;
    };
    int GetMonth() const {
        return month;
    };
    int GetDay() const {
        return day;
    };

    int year;
    int month;
    int day;
};

bool operator<(const Date& lhs, const Date& rhs) {
    return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
        vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
};

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        dates[date].insert(event);
    };
    bool DeleteEvent(const Date& date, const string& event) {
        if (!(dates.find(date) == dates.end())) {                           // if date exists 
            if (dates.at(date).find(event) != dates.at(date).end()) {       // if event exist in dates
                dates[date].erase(event);
                std::cout << "Deleted successfully" << std::endl;
                return 1;
            }
        }

        std::cout << "Event not found" << std::endl;
        return 0;



    };
    int  DeleteDate(const Date& date) {
        int size;
        if (!(dates.find(date) == dates.end())) {       //if date exists
            size = dates.at(date).size();
            dates.erase(date);
        }
        else {
            size = 0;
        }
        std::cout << "Deleted " << size << " events" << std::endl;
        return size;
    };

    void Find(const Date& date) const {
        if (!(dates.find(date) == dates.end())) {       // if date exists
            for (const std::string& str : dates.at(date)) {
                std::cout << str << std::endl;
            }
        }
    };

    void Print() const {
        for (const auto& events : dates) {
            int num_events = events.second.size();
            for (const std::string& str : events.second) {
                std::cout << std::setfill('0');
                std::cout << setw(4) << events.first.GetYear() << "-" << setw(2) << events.first.GetMonth() <<
                    "-" << setw(2) << events.first.GetDay() << " " << str << std::endl;
            }
        }
    };
private:
    map<Date, set<string>> dates;
};

bool is_number(const std::string& s)        //check is string may be integer 
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void check_number(char sign, std::string part_str, std::string str_for_wrong) {
    if (sign == '-' && !is_number(part_str)) {
        std::string part_str_wrong = part_str;
        if (part_str_wrong.substr(0, 1) == "+" || part_str_wrong.substr(0, 1) == "-") {
            part_str_wrong.erase(0, 1);
        }
        if (!is_number(part_str_wrong)) {
            std::cout << "Wrong date format: " << str_for_wrong << std::endl;
            exit(1);
        }
    }
}

std::vector<std::string> split(std::string str, char sign) {
    std::string str_for_wrong = str;
    std::vector<std::string> splitted = {};
    int num_sign = std::count(str.begin(), str.end(), sign);
    for (int i = 0; i < num_sign + 1; i++) {
        int pos = str.find(sign);
        int num_sign_now = std::count(str.begin(), str.end(), sign);
        if (pos != 0) {     // if it's not the first sign in string. Only for negative years
            std::string part_str = str.substr(0, pos);
            if (i == 1 && sign == ' ' && std::count(str.begin(), str.end(), '-') < 2) {
                // if we read date and it's not YYYY-MM-DD 
                std::cout << "Wrong date format: " << part_str << std::endl;
                exit(1);
            }
            check_number(sign, part_str, str_for_wrong);
            splitted.push_back(part_str);
            str = str.erase(0, pos + 1);

        }
        else {
            if (num_sign_now == 1) {
                splitted.push_back(str);
            }
            else {


                i++;        // for cycle remove one value from num_sign
                str = str.erase(0, 1);
                int pos = str.find(sign);

                std::string part_str = "-" + str.substr(0, pos);
                check_number(sign, part_str, str_for_wrong);

                splitted.push_back(part_str);
                str = str.erase(0, pos + 1);
            }
        }
    }
    return splitted;
}


Date parsing_date(std::string strings) {
    std::vector<std::string> date_ymd = split(strings, '-');     // parsing datetime
    if (std::stoi(date_ymd[1]) < 1 || std::stoi(date_ymd[1]) > 12) {
        std::cout << "Month value is invalid: " << date_ymd[1] << std::endl;
        exit(1);
    }
    if (std::stoi(date_ymd[2]) < 1 || std::stoi(date_ymd[2]) > 31) {
        std::cout << "Day value is invalid: " << date_ymd[2] << std::endl;
        exit(1);
    }
    Date date{ std::stoi(date_ymd[0]), std::stoi(date_ymd[1]), std::stoi(date_ymd[2]) };
    return date;
}


int main() {
    Database db;

    string command;
    while (getline(cin, command)) {
        if (command != "") {
            std::vector<std::string> strings = split(command, ' ');     // parsing input string
            std::string comm = strings[0];

            if (comm == "Add") {
                std::string datetime = strings[1];
                std::string event = strings[2];
                db.AddEvent(parsing_date(datetime), event);
            }
            else if (comm == "Find") {
                std::string datetime = strings[1];
                db.Find(parsing_date(datetime));
            }
            else if (comm == "Del") {
                if (strings.size() == 2) {
                    std::string datetime = strings[1];
                    db.DeleteDate(parsing_date(datetime));
                }
                else {
                    std::string datetime = strings[1];
                    std::string event = strings[2];
                    db.DeleteEvent(parsing_date(datetime), event);
                }
            }
            else if (comm == "Print") {
                db.Print();
            }
            else {
                std::cout << "Unknown command: " << comm << std::endl;
                exit(1);
            }
        }
    }

    return 0;
}