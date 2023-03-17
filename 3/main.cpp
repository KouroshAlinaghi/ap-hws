#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

#define DAYS_PER_COURSE 2
#define NUM_OF_CLASSES 2
#define NUM_OF_SESSIONS_PER_SUBJECT 2

typedef int Classroom; // [0, NUM_OF_CLASSES)

struct Time {
    int hour, minute;

    Time() {}
    Time(string time_format) {
        string h_s, m_s;
        bool passed_delim = false;
        for (char c : time_format) {
            if (c == ':') {
                passed_delim = true;
                continue;
            }
            if (passed_delim)
                m_s.push_back(c);
            else 
                h_s.push_back(c);
        }
        hour = stoi(h_s);
        minute = stoi(m_s);
    }

    string to_string() {
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << hour << ":" << std::setw(2) << std::setfill('0') << minute;
        return ss.str();
    }
};

typedef pair<Time, Time> Duration;

struct Course {
    string name;
    vector<int> present_days;
    Duration present_time_range;
};

struct Teacher {
    string name;
    vector<int> free_days;
    vector<string> subjects;
};

struct ClassTime {
    Course course;
    Teacher teacher;
    int weekday;
    Classroom classroom;
    Duration session;
};

typedef vector<ClassTime> Plan;

const vector<Duration> SESSIONS{ 
    make_pair(Time("7:30"), Time("9:00")), 
    make_pair(Time("9:30"), Time("11:00")), 
    make_pair(Time("11:30"), Time("13:00"))
};

const vector<string> WEEKDAYS = {
    "Saturday",
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday"
};

bool operator==(Time t1, Time t2) { return (t1.hour == t2.hour) && (t1.minute == t2.minute); }
bool operator>(Time t1, Time t2) { return (t1.hour * 60 + t1.minute) > (t2.hour * 60 + t2.minute); }
bool operator>=(Time t1, Time t2) { return t1 > t2 || t1 == t2; }
bool operator==(Course c1, Course c2) { return c1.name == c2.name; }
bool operator==(Teacher t1, Teacher t2) { return t1.name == t2.name; }
bool operator==(Duration d1, Duration d2) { return (d1.first == d2.first) && (d1.second == d2.second); }

int input_weekday() {
    string weekday;
    cin >> weekday;

    for (int i=0; i<(int)WEEKDAYS.size(); i++)
        if (WEEKDAYS[i] == weekday)
            return i;

    return -1;
}

Time input_time() {
    string time_format;
    cin >> time_format;
    return Time(time_format);
}

void input_teachers(vector<Teacher>& teachers) {
    int num_of_teachers;
    cin >> num_of_teachers;
    while (num_of_teachers--) {
        string name;
        cin >> name;
        int num_of_free_days;
        cin >> num_of_free_days;
        vector<int> free_days;
        while (num_of_free_days--)
            free_days.push_back(input_weekday());

        int num_of_subjects;
        cin >> num_of_subjects;
        vector<string> subjects;
        while (num_of_subjects--) {
            string subject;
            cin >> subject;
            subjects.push_back(subject);
        }

        teachers.push_back({ name, free_days, subjects });
    }
}

void input_courses(vector<Course>& courses) {
    int num_of_courses;
    cin >> num_of_courses;
    while (num_of_courses--) {
        string name;
        cin >> name;

        vector<int> present_days;
        for (int i=0; i<DAYS_PER_COURSE; i++)
            present_days.push_back(input_weekday());

        Time start_time = input_time();
        Time end_time = input_time();

        courses.push_back({ name, present_days, make_pair(start_time, end_time) });
    }
}

void input_teachers_and_courses(vector<Teacher>& teachers, vector<Course>& courses) {
    input_teachers(teachers);
    input_courses(courses);
}

void output_row(Plan plan, Course course, Classroom classroom) {
    for (ClassTime c_t : plan)
        if (c_t.course == course && c_t.classroom == classroom) {
            cout << c_t.teacher.name << ": " << c_t.session.first.to_string() << " " << c_t.session.second.to_string() << endl;
            return;
        }

    cout << "Not Found" << endl;
}

void output_plan(Plan plan, vector<Course> courses) {
    sort(courses.begin(), courses.end(), [](const Course c1, Course c2){ return c1.name < c2.name; });
    for (Course course : courses) {
        cout << course.name << endl;
        for (Classroom c=0; c<NUM_OF_CLASSES; c++)
            output_row(plan, course, c);
    }
}

bool does_teacher_teach(Teacher teacher, string course_name) {
    for (string c_n : teacher.subjects)
        if (c_n == course_name)
            return true;

    return false;
}

bool is_teacher_bussy(Plan plan, Teacher teacher, int weekday, Duration session) {
    for (ClassTime p : plan)
        if (p.teacher == teacher && p.weekday == weekday && p.session == session)
            return true;

    return false;
}

bool is_teacher_present(Teacher teacher, int weekday) {
    for (int d : teacher.free_days)
        if (d == weekday)
            return true;

    return false;
}

bool course_time_includes(Course course, Duration session) {
    return (session.first >= course.present_time_range.first) && (course.present_time_range.second >= session.second);
}

bool course_day_includes(Course course, int weekday) {
    for (int d : course.present_days)
        if (d == weekday)
            return true;

    return false;
}

bool is_class_full_at(Plan plan, Classroom classroom, Duration session, int weekday) {
    for (ClassTime p : plan)
        if (p.classroom == classroom && p.weekday == weekday && p.session == session)
            return true;
            
    return false;
}

int num_of_sessions(Plan plan, Classroom classroom, Course course) {
    int counter = 0;
    for (ClassTime c_t : plan) 
        if (c_t.classroom == classroom && c_t.course == course)
            counter++;

    return counter;
}

bool is_valid_for_course(Plan plan, Classroom classroom, int weekday, Duration session, Course course) {
    return (num_of_sessions(plan, classroom, course) < NUM_OF_SESSIONS_PER_SUBJECT) && (!is_class_full_at(plan, classroom, session, weekday)) && (course_time_includes(course, session)) && (course_day_includes(course, weekday));
}

bool is_valid_for_teacher(Plan plan, int weekday, Duration session, Course course, Teacher teacher) {
    return (!is_teacher_bussy(plan, teacher, weekday, session)) && (is_teacher_present(teacher, weekday)) && (does_teacher_teach(teacher, course.name));
}

bool is_valid_to_plan(Plan plan, Classroom classroom, int weekday, Duration session, Teacher teacher, Course course) {
    return is_valid_for_course(plan, classroom, weekday, session, course) 
        && is_valid_for_teacher(plan, weekday, session, course, teacher);
}

Course find_best_course(vector<Course> courses) {
    Course best_course = courses.at(0);
    for (Course course : courses)
        if (course.name < best_course.name)
            best_course = course;

    return best_course;
}

Teacher find_best_teacher(vector<Teacher> teachers) {
    Teacher best_teacher = teachers.at(0);
    for (Teacher teacher : teachers)
        if (teacher.free_days.size() == best_teacher.free_days.size()) {
            if (teacher.name < best_teacher.name)
                best_teacher = teacher;
        } else if (teacher.free_days.size() < best_teacher.free_days.size())
            best_teacher = teacher;

    return best_teacher;
}

bool can_fix_all_days(Plan plan, Classroom classroom, Duration session, Course course, Teacher teacher) {
    for (int weekday : course.present_days)
        if (!is_valid_to_plan(plan, classroom, weekday, session, teacher, course))
            return false;

    return true;
}

bool can_fix_all_days_for_course(Plan plan, vector<Teacher> teachers, Classroom classroom, Duration session, Course course) {
    int valid_teachers_counter = 0, is_teacher_valid = 0;
    for (Teacher teacher : teachers) {
        for (int weekday : course.present_days) {
            if (!is_valid_to_plan(plan, classroom, weekday, session, teacher, course)) {
                is_teacher_valid = 0;
                break;
            }
            is_teacher_valid = 1;
        }
        if (is_teacher_valid)
            valid_teachers_counter++;
    }
            
    return valid_teachers_counter > 0;
}

bool choose_best_course_and_teacher(
        Plan plan,
        vector<Course> courses,
        vector<Teacher> teachers,
        Duration session,
        int weekday,
        Classroom classroom,
        Teacher& teacher,
        Course& course
        ) {

    vector<Teacher> chosen_teachers;
    vector<Course> chosen_courses;
    for (Course c : courses)
        if (is_valid_for_course(plan, classroom, weekday, session, c) && can_fix_all_days_for_course(plan, teachers, classroom, session, c))
            chosen_courses.push_back(c);

    if (chosen_courses.size() == 0)
        return false;

    course = find_best_course(chosen_courses);

    for (Teacher t: teachers)
        if (can_fix_all_days(plan, classroom, session, course, t))
            chosen_teachers.push_back(t);

    if (chosen_teachers.size() == 0)
        return false;
    
    teacher = find_best_teacher(chosen_teachers);

    return true;
}

void debug(Plan plan) {
    for (ClassTime p : plan )
        cout << WEEKDAYS[p.weekday] << ", " << p.course.name << " by " << p.teacher.name << " at " << p.session.first.to_string() << " in class " << p.classroom << endl;
}

void fix_course(Plan& plan, Course course, Teacher teacher, Duration session, Classroom classroom) {
    for (int day : course.present_days)
        plan.push_back({ course, teacher, day, classroom, session });
}

Plan calc_plan(vector<Teacher> teahcers, vector<Course> courses) {
    Plan plan;
    Course course;
    Teacher teacher;
    for (Classroom classroom=0; classroom<NUM_OF_CLASSES; classroom++)
        for (int curr_day=0; curr_day<(int)WEEKDAYS.size(); curr_day++)
            for (Duration curr_session : SESSIONS )
                if (choose_best_course_and_teacher(plan, courses, teahcers, curr_session, curr_day, classroom, teacher, course))
                    fix_course(plan, course, teacher, curr_session, classroom);

    return plan;
}

int main() {
    vector<Teacher> teachers;
    vector<Course> courses;
    input_teachers_and_courses(teachers, courses);
    Plan plan = calc_plan(teachers, courses);
    output_plan(plan, courses);

    return 0;
}
