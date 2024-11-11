#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

struct TimeSlot {
    int start;
    int end;
};

int convertToMinutes(const string& time) {
    int hours, minutes;
    sscanf(time.c_str(), "%d:%d", &hours, &minutes);
    return hours * 60 + minutes;
}

string convertMinutesToHour(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    ostringstream oss;
    oss << hours << ":" << setw(2) << setfill('0') << mins;
    return oss.str();
}

vector<TimeSlot> updateFunction(const vector<TimeSlot>& schedule, const TimeSlot& dailyAct) {
    vector<TimeSlot> updatedSchedule;
    updatedSchedule.push_back({0, dailyAct.start}); // Early unavailable period
    updatedSchedule.insert(updatedSchedule.end(), schedule.begin(), schedule.end());
    updatedSchedule.push_back({dailyAct.end, 1439}); // Late unavailable period (23:59 in minutes)
    return updatedSchedule;
}

vector<TimeSlot> MergeFunction(const vector<TimeSlot>& schedule1, const vector<TimeSlot>& schedule2) {
    vector<TimeSlot> merged;
    size_t i = 0, j = 0;
    while (i < schedule1.size() && j < schedule2.size()) {
        if (schedule1[i].start <= schedule2[j].start) {
            merged.push_back(schedule1[i++]);
        } else {
            merged.push_back(schedule2[j++]);
        }
    }
    while (i < schedule1.size()) merged.push_back(schedule1[i++]);
    while (j < schedule2.size()) merged.push_back(schedule2[j++]);
    return merged;
}

vector<TimeSlot> softFunction(const vector<TimeSlot>& schedule) {
    vector<TimeSlot> sortedSchedule = schedule;
    sort(sortedSchedule.begin(), sortedSchedule.end(), [](const TimeSlot& a, const TimeSlot& b) {
        return a.start < b.start;
    });
    return sortedSchedule;
}

vector<TimeSlot> matchedAvailabilities(const vector<TimeSlot>& schedule, int duration) {
    vector<TimeSlot> availabilities;
    int end_time_of_last_meeting = schedule[0].end;

    for (size_t i = 1; i < schedule.size(); ++i) {
        int start_time_of_next_meeting = schedule[i].start;
        if (start_time_of_next_meeting - end_time_of_last_meeting >= duration) {
            availabilities.push_back({end_time_of_last_meeting, start_time_of_next_meeting});
        }
        end_time_of_last_meeting = max(end_time_of_last_meeting, schedule[i].end);
    }

    return availabilities;
}

vector<TimeSlot> groupSchedule(const vector<TimeSlot>& person1, const TimeSlot& person1Avil,
                               const vector<TimeSlot>& person2, const TimeSlot& person2Avil, int duration) {
    vector<TimeSlot> updatedSchedule1 = updateFunction(person1, person1Avil);
    vector<TimeSlot> updatedSchedule2 = updateFunction(person2, person2Avil);
    vector<TimeSlot> mergedSchedule = MergeFunction(updatedSchedule1, updatedSchedule2);
    vector<TimeSlot> sortedSchedules = softFunction(mergedSchedule);
    return matchedAvailabilities(sortedSchedules, duration);
}

int main() {
    // ============================== TEST CASE 1 ==============================
    cout << "********************* TEST CASE 1 *********************\n";
    vector<TimeSlot> person1 = {
        {convertToMinutes("7:00"), convertToMinutes("8:30")},
        {convertToMinutes("12:00"), convertToMinutes("13:00")},
        {convertToMinutes("16:00"), convertToMinutes("18:00")}
    };

    vector<TimeSlot> person2 = {
        {convertToMinutes("9:00"), convertToMinutes("10:30")},
        {convertToMinutes("12:20"), convertToMinutes("13:30")},
        {convertToMinutes("14:00"), convertToMinutes("15:00")},
        {convertToMinutes("16:00"), convertToMinutes("17:00")}
    };

    TimeSlot person1Avil = {convertToMinutes("9:00"), convertToMinutes("19:00")};
    TimeSlot person2Avil = {convertToMinutes("10:00"), convertToMinutes("18:30")};
    int duration = 30;

    vector<TimeSlot> availableTimes = groupSchedule(person1, person1Avil, person2, person2Avil, duration);
    for (const TimeSlot& slot : availableTimes) {
        cout << convertMinutesToHour(slot.start) << " - " << convertMinutesToHour(slot.end) << endl;
    }

    // ============================== TEST CASE 2 ==============================
    cout << "********************* TEST CASE 2 *********************\n";
    vector<TimeSlot> person3 = {
        {convertToMinutes("6:00"), convertToMinutes("9:30")},
        {convertToMinutes("11:00"), convertToMinutes("13:00")},
        {convertToMinutes("17:00"), convertToMinutes("18:00")}
    };

    vector<TimeSlot> person4 = {
        {convertToMinutes("7:00"), convertToMinutes("9:15")},
        {convertToMinutes("11:30"), convertToMinutes("13:00")},
        {convertToMinutes("14:30"), convertToMinutes("16:00")},
        {convertToMinutes("17:00"), convertToMinutes("20:00")}
    };

    TimeSlot person3Avil = {convertToMinutes("7:00"), convertToMinutes("19:00")};
    TimeSlot person4Avil = {convertToMinutes("8:00"), convertToMinutes("17:15")};
    int duration1 = 30;

    vector<TimeSlot> availableTimes2 = groupSchedule(person3, person3Avil, person4, person4Avil, duration1);
    for (const TimeSlot& slot2 : availableTimes2) {
        cout << convertMinutesToHour(slot2.start) << " - " << convertMinutesToHour(slot2.end) << endl;
    }

    // ============================== TEST CASE 3 ==============================
    cout << "********************* TEST CASE 3 *********************\n";
    vector<TimeSlot> person5 = {
        {convertToMinutes("8:00"), convertToMinutes("10:00")},
        {convertToMinutes("14:00"), convertToMinutes("15:30")},
        {convertToMinutes("18:30"), convertToMinutes("20:00")}
    };

    vector<TimeSlot> person6 = {
        {convertToMinutes("9:30"), convertToMinutes("11:00")},
        {convertToMinutes("13:00"), convertToMinutes("14:30")},
        {convertToMinutes("17:00"), convertToMinutes("19:00")}
    };

    TimeSlot person5Avil = {convertToMinutes("7:00"), convertToMinutes("21:00")};
    TimeSlot person6Avil = {convertToMinutes("8:00"), convertToMinutes("20:00")};
    int duration2 = 45;

    vector<TimeSlot> availableTimes3 = groupSchedule(person5, person5Avil, person6, person6Avil, duration2);
    for (const TimeSlot& slot3 : availableTimes3) {
        cout << convertMinutesToHour(slot3.start) << " - " << convertMinutesToHour(slot3.end) << endl;
    }

    // ============================== TEST CASE 4 ==============================
    cout << "********************* TEST CASE 4 *********************\n";
    vector<TimeSlot> person7 = {
        {convertToMinutes("9:00"), convertToMinutes("11:00")},
        {convertToMinutes("14:00"), convertToMinutes("16:00")}
    };

    vector<TimeSlot> person8 = {
        {convertToMinutes("10:00"), convertToMinutes("12:00")},
        {convertToMinutes("13:30"), convertToMinutes("15:30")}
    };

    TimeSlot person7Avil = {convertToMinutes("9:00"), convertToMinutes("17:00")};
    TimeSlot person8Avil = {convertToMinutes("10:00"), convertToMinutes("16:00")};
    int duration3 = 60;

    vector<TimeSlot> availableTimes4 = groupSchedule(person7, person7Avil, person8, person8Avil, duration3);
    for (const TimeSlot& slot4 : availableTimes4) {
        cout << convertMinutesToHour(slot4.start) << " - " << convertMinutesToHour(slot4.end) << endl;
    }

    // ============================== TEST CASE 5 ==============================
    cout << "********************* TEST CASE 5 *********************\n";
    vector<TimeSlot> person9 = {
        {convertToMinutes("9:00"), convertToMinutes("12:00")},
        {convertToMinutes("14:00"), convertToMinutes("15:30")}
    };

    vector<TimeSlot> person10 = {
        {convertToMinutes("10:00"), convertToMinutes("13:00")},
        {convertToMinutes("16:00"), convertToMinutes("18:00")}
    };

    TimeSlot person9Avil = {convertToMinutes("9:00"), convertToMinutes("18:00")};
    TimeSlot person10Avil = {convertToMinutes("9:30"), convertToMinutes("18:30")};
    int duration4 = 30;

    vector<TimeSlot> availableTimes5 = groupSchedule(person9, person9Avil, person10, person10Avil, duration4);
    for (const TimeSlot& slot5 : availableTimes5) {
        cout << convertMinutesToHour(slot5.start) << " - " << convertMinutesToHour(slot5.end) << endl;
    }

    // ============================== TEST CASE 6 ==============================
    cout << "********************* TEST CASE 6 *********************\n";
    vector<TimeSlot> person11 = {
        {convertToMinutes("7:00"), convertToMinutes("9:00")},
        {convertToMinutes("10:30"), convertToMinutes("12:30")}
    };

    vector<TimeSlot> person12 = {
        {convertToMinutes("8:00"), convertToMinutes("10:00")},
        {convertToMinutes("13:00"), convertToMinutes("15:00")}
    };

    TimeSlot person11Avil = {convertToMinutes("7:00"), convertToMinutes("15:30")};
    TimeSlot person12Avil = {convertToMinutes("8:00"), convertToMinutes("15:00")};
    int duration5 = 45;

    vector<TimeSlot> availableTimes6 = groupSchedule(person11, person11Avil, person12, person12Avil, duration5);
    for (const TimeSlot& slot6 : availableTimes6) {
        cout << convertMinutesToHour(slot6.start) << " - " << convertMinutesToHour(slot6.end) << endl;
    }

    // ============================== TEST CASE 7 ==============================
    cout << "********************* TEST CASE 7 *********************\n";
    vector<TimeSlot> person13 = {
        {convertToMinutes("6:00"), convertToMinutes("9:00")},
        {convertToMinutes("11:00"), convertToMinutes("13:00")},
        {convertToMinutes("16:00"), convertToMinutes("18:00")}
    };

    vector<TimeSlot> person14 = {
        {convertToMinutes("7:00"), convertToMinutes("8:00")},
        {convertToMinutes("13:00"), convertToMinutes("15:00")}
    };

    TimeSlot person13Avil = {convertToMinutes("6:00"), convertToMinutes("18:00")};
    TimeSlot person14Avil = {convertToMinutes("7:00"), convertToMinutes("15:30")};
    int duration6 = 30;

    vector<TimeSlot> availableTimes7 = groupSchedule(person13, person13Avil, person14, person14Avil, duration6);
    for (const TimeSlot& slot7 : availableTimes7) {
        cout << convertMinutesToHour(slot7.start) << " - " << convertMinutesToHour(slot7.end) << endl;
    }

    // ============================== TEST CASE 8 ==============================
    cout << "********************* TEST CASE 8 *********************\n";
    vector<TimeSlot> person15 = {
        {convertToMinutes("10:00"), convertToMinutes("12:00")},
        {convertToMinutes("15:00"), convertToMinutes("17:00")}
    };

    vector<TimeSlot> person16 = {
        {convertToMinutes("11:00"), convertToMinutes("13:00")},
        {convertToMinutes("16:00"), convertToMinutes("18:00")}
    };

    TimeSlot person15Avil = {convertToMinutes("9:00"), convertToMinutes("19:00")};
    TimeSlot person16Avil = {convertToMinutes("10:00"), convertToMinutes("18:30")};
    int duration7 = 45;

    vector<TimeSlot> availableTimes8 = groupSchedule(person15, person15Avil, person16, person16Avil, duration7);
    for (const TimeSlot& slot8 : availableTimes8) {
        cout << convertMinutesToHour(slot8.start) << " - " << convertMinutesToHour(slot8.end) << endl;
    }

    // ============================== TEST CASE 9 ==============================
    cout << "********************* TEST CASE 9 *********************\n";
    vector<TimeSlot> person17 = {
        {convertToMinutes("8:00"), convertToMinutes("10:00")},
        {convertToMinutes("12:00"), convertToMinutes("14:00")}
    };

    vector<TimeSlot> person18 = {
        {convertToMinutes("9:00"), convertToMinutes("11:00")},
        {convertToMinutes("13:00"), convertToMinutes("15:00")}
    };

    TimeSlot person17Avil = {convertToMinutes("8:00"), convertToMinutes("16:00")};
    TimeSlot person18Avil = {convertToMinutes("9:00"), convertToMinutes("17:00")};
    int duration8 = 60;

    vector<TimeSlot> availableTimes9 = groupSchedule(person17, person17Avil, person18, person18Avil, duration8);
    for (const TimeSlot& slot9 : availableTimes9) {
        cout << convertMinutesToHour(slot9.start) << " - " << convertMinutesToHour(slot9.end) << endl;
    }

    // ============================== TEST CASE 10 ==============================
    cout << "********************* TEST CASE 10 *********************\n";
    vector<TimeSlot> person19 = {
        {convertToMinutes("8:00"), convertToMinutes("9:30")},
        {convertToMinutes("10:30"), convertToMinutes("13:00")},
        {convertToMinutes("14:30"), convertToMinutes("16:00")}
    };

    vector<TimeSlot> person20 = {
        {convertToMinutes("9:00"), convertToMinutes("11:00")},
        {convertToMinutes("12:00"), convertToMinutes("13:30")},
        {convertToMinutes("15:30"), convertToMinutes("18:00")}
    };

    TimeSlot person19Avil = {convertToMinutes("8:00"), convertToMinutes("18:00")};
    TimeSlot person20Avil = {convertToMinutes("9:00"), convertToMinutes("17:00")};
    int duration9 = 30;

    vector<TimeSlot> availableTimes10 = groupSchedule(person19, person19Avil, person20, person20Avil, duration9);
    for (const TimeSlot& slot10 : availableTimes10) {
        cout << convertMinutesToHour(slot10.start) << " - " << convertMinutesToHour(slot10.end) << endl;
    }

    return 0;
}


