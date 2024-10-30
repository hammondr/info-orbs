#ifndef WORDLEWIDGET_H
#define WORDLEWIDGET_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <TJpg_Decoder.h>
#include <config.h>
#include <globalTime.h>
#include <math.h>
#include <widget.h>


class WordleWidget : public Widget {
   public:
    WordleWidget(ScreenManager& manager);
    ~WordleWidget() override;
    void setup() override;
    void update(bool force = false) override;
    void draw(bool force = false) override;
    void changeMode() override;

   private:
    String generateWordleDate();
    String scrapeWordleAnswer(const String);
    bool getWordleData();
    bool getWordleData2();
    int getClockStamp();
    bool isChanged();
    void wordleText(int16_t b, int16_t s, int16_t t);
    int calculateWordleDay(int year, int month, int day);

    GlobalTime* m_time;
    int8_t m_mode;
    String wordleWord;
    String previousWordleWord;

    const long m_updateDelay = 600000;  // Wordle refresh rate ... 600000ms = 10 min
    unsigned long m_lastUpdate = 0;

    const int centre = 120;  // centre location of the screen(240x240)

    int m_clockStamp = 0;

    String httpRequestAddressBase = "https://www.cnet.com/tech/gaming/todays-wordle-hints-answer-and-help-for-";
};
#endif  // WORDLEWIDGET_H
