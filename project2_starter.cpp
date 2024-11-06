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

    // ============================== THIS IS FOR TESTING PURPOSE ==============================
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

    // Daily availability for each person
    TimeSlot person1Avil = {convertToMinutes("9:00"), convertToMinutes("19:00")};
    TimeSlot person2Avil = {convertToMinutes("10:00"), convertToMinutes("18:30")};

    // Duration of the proposed meeting in minutes
    int duration = 30;

    // Running the function with the test case
    vector<TimeSlot> availableTimes = groupSchedule(person1, person1Avil, person2, person2Avil, duration);

    // Displaying the result
    cout << "Available times for the meeting:\n";
    for (const TimeSlot& slot : availableTimes) {
        cout << convertMinutesToHour(slot.start) << " - " << convertMinutesToHour(slot.end) << endl;
    }

    // ==========================================================================
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

    // Daily availability for each person
    TimeSlot person3Avil = {convertToMinutes("7:00"), convertToMinutes("19:00")};
    TimeSlot person4Avil = {convertToMinutes("8:00"), convertToMinutes("17:15")};

    // Duration of the proposed meeting in minutes
    int duration1 = 30;

    // Running the function with the test case
    vector<TimeSlot> availableTimes2 = groupSchedule(person3, person3Avil, person4, person4Avil, duration1);

    // Displaying the result
    cout << "Available times for the meeting:\n";
    for (const TimeSlot& slot2 : availableTimes2) {
        cout << convertMinutesToHour(slot2.start) << " - " << convertMinutesToHour(slot2.end) << endl;
    }


    return 0;
}
