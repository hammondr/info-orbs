#include "widgets/wordleWidget.h"

#include <iostream>
#include <chrono>
#include <ctime>

// #include <string>

WordleWidget::WordleWidget(ScreenManager &manager) : Widget(manager) {
//    m_mode = MODE_HIGHS;
}

WordleWidget::~WordleWidget() {
}

void WordleWidget::changeMode() {
    update(true);
}

void WordleWidget::setup() {
    m_lastUpdate = millis() - m_updateDelay + 1000;
    m_time = GlobalTime::getInstance();

    previousWordleWord = "";
    wordleWord = "";
}

void WordleWidget::draw(bool force) {
    wordleWord = "UNSET";
//    if (isChanged()) {
        wordleText(TFT_WHITE, 0x6d4c, TFT_WHITE);
//    }
}

bool WordleWidget::isChanged() {
    return (wordleWord != previousWordleWord);
}

void WordleWidget::update(bool force) {
    wordleWord = "UNSET";
    //getWordleData();
    // if (force || m_lastUpdate == 0 || (millis() - m_lastUpdate) >= m_updateDelay) {
    //     setBusy(true);
    //     if (force) {
    //         int retry = 0;
    //         while (!getWordleData() && retry++ < MAX_RETRIES);
    //     } else {
    //         getWordleData();
    //     }
    //     setBusy(false);
    //     m_lastUpdate = millis();
    // }
}

// bool WordleWidget::getWordleData2() {
//     std::string url = "https://www.cnet.com/tech/gaming/todays-wordle-hints-answer-and-help-for-oct-26-1225/";
//     std::string wordleAnswer = scrapeWordleAnswer(url);
//     return true;
// }

bool WordleWidget::getWordleData() {
    HTTPClient http;
    String url = httpRequestAddressBase + generateWordleDate();

    Serial.printf("Checking Wordle URL: %s\n", url);

    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {  // Check for the returning code

        Serial.printf("Scraping for wordle solution.\n");

//        if (!error) {
            wordleWord = scrapeWordleAnswer(http.getString());
//        }
        http.end();
    } else {
        // Handle HTTP request error
        Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
        http.end();
        return false;
    }
    return true;
}


// This displays the letters of the wordle. Pass in background color, square color, text color
void WordleWidget::wordleText(int16_t b, int16_t s, int16_t t) {

    // Iterate using an index variable to show the position within the string
    for (size_t i = 0; i < wordleWord.length(); i++) {
        char currentChar = wordleWord[i];

        m_manager.selectScreen(i);
        TFT_eSPI &display = m_manager.getDisplay();

        display.fillScreen(b);
        display.setTextColor(t);
        display.setTextSize(17);
        display.setTextDatum(BC_DATUM);
        display.fillRect(60, 60, centre, centre, s);  // rgb565 colors
        String letter = String(wordleWord.charAt(i));
        display.drawString(letter, centre, 80, 2);
    }
}

//---------------------------

// Callback function for libcurl to write data
// size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
//     userp->append((char*)contents, size * nmemb);
//     return size * nmemb;
// }

// Function to find the Wordle answer in the HTML text
String WordleWidget::scrapeWordleAnswer(const String html) {
    String target = "Today's Wordle answer is ";
    size_t pos = html.indexOf(target);
    if (pos != -1) {
        pos += target.length();
        size_t endPos = html.indexOf(".", pos);
        return html.substring(pos, endPos - pos);
    }

    return "ERROR";
}

// Function to calculate days since a specific start date
int WordleWidget::calculateWordleDay(int year, int month, int day) {
    struct tm startDate = {0};
    startDate.tm_year = 2021 - 1900;  // Wordle start year
    startDate.tm_mon = 6 - 1;         // Wordle start month (June)
    startDate.tm_mday = 19;           // Wordle start day

    struct tm currentDate = {0};
    currentDate.tm_year = year - 1900;
    currentDate.tm_mon = month - 1;
    currentDate.tm_mday = day;

    time_t start = mktime(&startDate);
    time_t now = mktime(&currentDate);

    double seconds = difftime(now, start);
    int days = seconds / (60 * 60 * 24);
    return days;
}

// Function to generate the date string in the desired format
String WordleWidget::generateWordleDate() {
    // Get today's date
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm* parts = std::localtime(&now_c);

    int year = parts->tm_year + 1900;
    int month = parts->tm_mon + 1;
    int day = parts->tm_mday;

    String monthNames[] = { "jan", "feb", "mar", "apr", "may", "jun",
                            "jul", "aug", "sep", "oct", "nov", "dec" };

    int wordleDay = calculateWordleDay(year, month, day);

    // Generate formatted string using Arduino String class
    String formattedDate = monthNames[month - 1] + "-" +
                           String(day) + "-" +
                           String(wordleDay);
    return formattedDate;
}

// int main() {
//     std::string url = "https://www.cnet.com/tech/gaming/todays-wordle-hints-answer-and-help-for-oct-26-1225/";
//     std::string wordleAnswer = scrapeWordleAnswer(url);

//     if (wordleAnswer != "Not found") {
//         std::cout << wordleAnswer << std::endl;
//     } else {
//         std::cout << "Could not find the Wordle answer on the page." << std::endl;
//     }

//     return 0;
// }