#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <fstream>

using namespace std;

#define PW_FILENAME "positive-words.txt"

vector<string> split(string line, char delimiter);

struct Date {
    int year, month, day;

    Date() {};

    // Contrsuctor from string of format "day/month/year"
    Date(string date_string) {
        day   = stoi(split(date_string, '/')[0]);
        month = stoi(split(date_string, '/')[1]);
        year  = stoi(split(date_string, '/')[2]);
    }

    string to_string() {
        return ::to_string(day) + "/" + ::to_string(month) + "/" + ::to_string(year);
    }
};

struct Diary {
    Date date;
    string content;
};

// Splitting given string to a vector<string> using given delimiter
vector<string> split(string line, char delimiter) {
    vector<string> words;
    int start = 0, end = 0, i = 0;
    while (i <= line.size()) {  
        if (line[i] == delimiter || i == line.size() || line[i] == '\n') {  
            end = i;  
            string word = "";  
            word.append(line, start, end - start);  
            words.push_back(word);
            start = end + 1;  
        }  
        i++;  
    }     
    return words;
}

string diary_preview(Diary diary) {
    string content_preview;
    content_preview = diary.content.size() >= 20 ? content_preview.append(diary.content, 0, 20) + "..." : diary.content;
    return diary.date.to_string() + "\n" + content_preview;
}

// Defining == operator for Date
bool operator==(const Date& d1, const Date& d2) {
    return (d1.day == d2.day) && (d1.month == d2.month) && (d1.year == d2.year);
}

// Sets current_date to given date
void set_start_day(Date &curr_date, string date_string) {
    curr_date = Date(date_string);
}

// Print given day's diaries
void show_day(string date_string, vector<Diary> diaries) {
    Date date = Date(date_string);
    for (auto diary: diaries) {
        if (diary.date == date)
            cout << diary.content;
    }
}

// Pushes a line to given date's diary (creates new diary if it's new)
void push_to_diary(string line, Date date, vector<Diary>& diaries) {
    for (auto &diary: diaries)
        if (diary.date == date) {
            diary.content = diary.content + line + "\n";
            return;
        }

    Diary new_diary;
    new_diary.date = date;
    new_diary.content = line + "\n";

    diaries.push_back(new_diary);

    return;
}

bool compare_diaries_by_content_size(Diary a, Diary b) {
    return a.content.size() < b.content.size();
}

void show_longest_day(vector<Diary> diaries) {
    sort(diaries.begin(), diaries.end(), compare_diaries_by_content_size);
    cout << diary_preview(diaries.back()) << endl;
}

// returns a vector of file's lines
vector<string> extract_words_from_file() {
    vector<string> words;
    string word;
    fstream file;
    file.open(PW_FILENAME, ios::in); 

    while (getline(file, word))
        words.push_back(word);
        
    file.close(); 

    return words;
}

// Returns number of good_words in given string
int positive_words_frequency(string content) {
    int f = 0;
    vector<string> positive_words = extract_words_from_file(), content_words = split(content, ' ');
    for (auto positive_word: positive_words)
        if (::count(content_words.begin(), content_words.end(), positive_word))
            f++;

    return f;
}

bool operator>(const Date& a, const Date& b) {
    return a.day + a.month*30 + a.year*365 > b.day + b.month*30 + b.year*365;
}

bool operator<(const Diary& a, const Diary& b) {
    if (positive_words_frequency(a.content) == positive_words_frequency(b.content))
        return a.date > b.date;
    else
        return positive_words_frequency(a.content) < positive_words_frequency(b.content);
}

void show_best_day(vector<Diary> diaries) {
    sort(diaries.begin(), diaries.end());
    cout << diary_preview(diaries.back()) << endl;
}

int main() {
    string input;
    Date current_date("1/1/1970");
    vector<Diary> diaries;

    while (getline(cin, input)) {
        string first_word = split(input, ' ')[0];

        if (first_word == "start_day") {
            set_start_day(current_date, split(input, ' ')[1]);
        } else if (first_word == "show_day") {
            show_day(split(input, ' ')[1], diaries);
        } else if (first_word == "show_the_longest_day") {
            show_longest_day(diaries);
        } else if (first_word == "show_the_best_day") {
            show_best_day(diaries);
        } else {
            push_to_diary(input, current_date, diaries);
        }
    }

    return 0;
}
