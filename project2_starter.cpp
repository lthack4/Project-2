#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct TimeSlot {
    string start;
    string end;
};

int convertToMinutes(const string& time) {
    int hours, minutes;
    sscanf(time.c_str(), "%d:%d", &hours, &minutes);
    return hours * 60 + minutes;
}

string convertMinutesToHour(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    char buffer[6];
    sprintf(buffer, "%02d:%02d", hours, mins);
    return string(buffer);
}

vector<TimeSlot> updateSchedule(const vector<TimeSlot>& schedule, const TimeSlot& dailyAct) {
    vector<TimeSlot> updatedSchedule;
    updatedSchedule.push_back({"0:00", dailyAct.start});
    updatedSchedule.insert(updatedSchedule.end(), schedule.begin(), schedule.end());
    updatedSchedule.push_back({dailyAct.end, "23:59"});
    return updatedSchedule;
}

vector<TimeSlot> mergeSchedules(const vector<TimeSlot>& schedule1, const vector<TimeSlot>& schedule2) {
    vector<TimeSlot> merged;
    int i = 0, j = 0;
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

vector<TimeSlot> sortedAvailabilities(const vector<TimeSlot>& schedule) {
    vector<TimeSlot> availabilities;
    for (int i = 0; i < schedule.size() - 1; ++i) {
        if (schedule[i].end < schedule[i + 1].start) {
            availabilities.push_back({schedule[i].end, schedule[i + 1].start});
        }
    }
    return availabilities;
}

vector<TimeSlot> matchedAvailabilities(const vector<TimeSlot>& schedule, int duration) {
    vector<TimeSlot> result;
    for (const auto& slot : schedule) {
        int start = convertToMinutes(slot.start);
        int end = convertToMinutes(slot.end);
        if (end - start >= duration) {
            result.push_back({convertMinutesToHour(start), convertMinutesToHour(end)});
        }
    }
    return result;
}

void groupSchedule(const vector<TimeSlot>& pers1Schedule, const TimeSlot& pers1DailyAct,
                   const vector<TimeSlot>& pers2Schedule, const TimeSlot& pers2DailyAct, int duration) {
    vector<TimeSlot> updatedSchedule1 = updateSchedule(pers1Schedule, pers1DailyAct);
    vector<TimeSlot> updatedSchedule2 = updateSchedule(pers2Schedule, pers2DailyAct);
    vector<TimeSlot> mergedSchedule = mergeSchedules(updatedSchedule1, updatedSchedule2);
    vector<TimeSlot> sortedSchedules = sortedAvailabilities(mergedSchedule);
    vector<TimeSlot> availabilities = matchedAvailabilities(sortedSchedules, duration);

    cout << "Available Time Slots:\n";
    for (const auto& timeSlot : availabilities) {
        cout << timeSlot.start << " - " << timeSlot.end << endl;
    }
}

int main() {
    vector<TimeSlot> pers1Schedule = {{"7:00", "8:30"}, {"12:00", "13:00"}, {"16:00", "18:00"}};
    vector<TimeSlot> pers2Schedule = {{"9:00", "10:30"}, {"12:30", "13:30"}, {"14:30", "15:00"}, {"16:00", "17:00"}};
    TimeSlot pers1DailyAct = {"9:00", "19:00"};
    TimeSlot pers2DailyAct = {"10:00", "18:30"};
    int duration = 30; // Duration in minutes

    groupSchedule(pers1Schedule, pers1DailyAct, pers2Schedule, pers2DailyAct, duration);

    /* Must display:
                10:30, 12:00
                15:00, 16:00
                18:00, 18:30
                 */

    return 0;
}
