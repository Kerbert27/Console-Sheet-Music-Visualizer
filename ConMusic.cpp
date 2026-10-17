#include <stdio.h>
#include <iostream>
#include <set>
#include <map>
#include "olcConsoleGameEngine.h"

using namespace std;



enum PITCH {
    C = 1, D, E, F, G, A, B
};

enum LENGTH {
    WHOLE = 1, HALF = 2, QUARTER = 4, EIGHTH = 8, REST
};

enum CLEF {
    TREBLE, TENOR, BASS
};

enum ACCIDENTAL {
    SHARP, FLAT, NATURAL
};

map<PITCH, float> frequencies = {
    {C, 32}, {D, 36}, {E, 41}, {F, 43}, {G, 49}, {A, 55}, {B, 61}
};

struct Note {
    PITCH pitch;
    ACCIDENTAL accidental = NATURAL;
    int beat;
    LENGTH length;
    int octave;

    Note() = default;

    Note(PITCH p, int o) {
        pitch = p;
        octave = o;
        beat = 1;
        length = QUARTER;
    }

    Note(PITCH p, int b, LENGTH l, int o) {
        pitch = p;
        beat = b;
        length = l;
        octave = o;
    }

    bool operator<(const Note& other) const{
        return this->octave < other.octave || (this->octave == other.octave && this->pitch < other.pitch);
    }

    Note operator+(const int& val) const{
        Note sum = *this;
        sum.pitch = (PITCH)((sum.pitch + val) % 7);
        sum.octave += floor((this->pitch + val) / 8);
        return sum;
    }

    Note operator-(const int& val) const {
        Note sum = *this;
        sum.pitch = (PITCH)((sum.pitch - val) % 7);
        sum.octave -= floor((this->pitch - val) / 8);
        return sum;
    }
};

class ConsoleMusic : public olcConsoleGameEngine {
private:
    vector<set<Note*>> notes;
    int TimeSignature = 3;
    int BeatSignature = 4;
    float Time;
    float lastTime = 0;
    int LeftMargin;
    int RightMargin;
    int LineWidth;
    int MeasureWidth;
    int BeatWidth;
    int BeatPadding;
    int StaffGap = 20;
    int TopLine;
    int BottomLine;
    int StaffOffset;
    int CurrentBeat;
    int CurrentOctave;
    CLEF clef;
    Note* TopNote, *MiddleNote;
 

public:
    ConsoleMusic() {
        m_sAppName = L"ConsoleMusic";
    }

    void SetTimeSignature(int t, int b) {
        TimeSignature = t;
        BeatSignature = b;
    }

protected:
    virtual bool OnUserCreate() {
        clef = CLEF::TENOR;
        switch (clef) {
        case TREBLE:
            TopNote = new Note(C, 6);
            MiddleNote = new Note(B, 4);
            break;
        case TENOR:
            TopNote = new Note(D, 5);
            MiddleNote = new Note(C, 4);
            break;
        case BASS:
            TopNote = new Note(E, 4);
            MiddleNote = new Note(D, 3);
            break;
        }
        CurrentOctave = MiddleNote->octave;
        LeftMargin = 0;
        RightMargin = ScreenWidth();
        LineWidth = RightMargin - LeftMargin;
        BeatWidth = 8;
        BeatPadding = 3;
        StaffGap = 12;  //MUST BE DIVISIBLE BY 2
        MeasureWidth = TimeSignature * (BeatWidth + BeatPadding);
        StaffOffset = ScreenHeight() / 2;
        TopLine = StaffOffset + (StaffGap * 4);
        BottomLine = StaffOffset;
        CurrentBeat = 1;
        notes.resize(LineWidth / MeasureWidth * TimeSignature);
        HighlightBeat(BG_WHITE);
        Time = 0; 
        return 1;
    }

    void MoveBeat(bool dir) {
        HighlightBeat(BG_BLACK);
        if (dir) {
            CurrentBeat++;
        }
        else CurrentBeat--;
        HighlightBeat(BG_WHITE);
    }

    void MoveOctave(bool dir) {
        HighlightBeat(BG_BLACK);
        if (dir) {
            CurrentOctave++;
        }
        else CurrentOctave--;
        HighlightBeat(BG_WHITE);
    }

    void DisplayInfo() {
        cout << "Current Octave" + CurrentOctave << endl;
    }

    void DrawSin(float x1, float x2, int y, float amp) {
        Fill(0, 0, ScreenWidth(), 150, 9608, BG_BLACK);
        int lastX, lastY;
        for (int i = x1; i <= x2; i++) {
            int k = 0;
            for (const auto note : notes[CurrentBeat]) {
                k += amp * sin(2 * 3.141592f * (i / x2 - x1) * (frequencies[note->pitch]*note->octave)/10 + Time);
            }
            if(i != x1) DrawLine(lastX, lastY + y, i, k + y);
            lastX = i;
            lastY = k;
        }

    }

    virtual bool OnUserUpdate(float elapsedTime) {

        if (m_keys[VK_RIGHT].bPressed) {
            MoveBeat(1);
        }
        if (m_keys[VK_LEFT].bPressed) {
            MoveBeat(0);
        }
        if (m_keys[VK_UP].bPressed) {
            MoveOctave(1);
        }
        if (m_keys[VK_DOWN].bPressed) {
            MoveOctave(0);
        }

        if (m_keys['A'].bPressed) {
            notes[CurrentBeat].emplace(new Note(A, CurrentBeat, QUARTER, CurrentOctave));
            MoveBeat(1);
        }
        if (m_keys['B'].bPressed) {
            notes[CurrentBeat].emplace(new Note(B, CurrentBeat, QUARTER, CurrentOctave));
            MoveBeat(1);
        }
        if (m_keys['C'].bPressed) {
            notes[CurrentBeat].emplace(new Note(C, CurrentBeat, QUARTER, CurrentOctave));
            MoveBeat(1);;
        }
        if (m_keys['D'].bPressed) {
            notes[CurrentBeat].emplace(new Note(D, CurrentBeat, QUARTER, CurrentOctave));
            MoveBeat(1);
        }
        if (m_keys['E'].bPressed) {
            notes[CurrentBeat].emplace(new Note(E, CurrentBeat, QUARTER, CurrentOctave));
            MoveBeat(1);
        }
        if (m_keys['F'].bPressed) {
            notes[CurrentBeat].emplace(new Note(F, CurrentBeat, QUARTER, CurrentOctave));
            MoveBeat(1);
        }
        if (m_keys['G'].bPressed) {
            notes[CurrentBeat].emplace(new Note(G, CurrentBeat, QUARTER, CurrentOctave));
            MoveBeat(1);
        }
        if (m_keys[VK_BACK].bPressed) {
            notes[CurrentBeat].clear();
            Fill(0, 0, ScreenWidth(), ScreenHeight(), 9608, BG_BLACK);
            HighlightBeat(BG_WHITE);
        }

        if (!notes[CurrentBeat].empty()) {
            DrawSin(LeftMargin, RightMargin, 80, 10);
            Time += 0.1;
        }

        for (auto& beat : notes) {
            for (const auto note : beat) {
                DrawNote(note);
            }
        }

        DrawStaff();
        return 1;
    }

    void HighlightBeat(COLOUR color) {
        int x1 = (CurrentBeat - 1) * (BeatWidth + BeatPadding);
        int x2 = CurrentBeat * (BeatWidth + BeatPadding);
        int yBottom = (BottomLine - StaffGap*2) + ((StaffGap / 2) * (((TopNote->octave - CurrentOctave) * 7) + TopNote->pitch));;
        int yTop = (BottomLine - StaffGap*2) + ((StaffGap / 2) * (((TopNote->octave - CurrentOctave) * 7) + TopNote->pitch - 7));
        Fill(x1, yTop, x2, yBottom, 9608, color);
    }

    void DrawStaff() {
        for (int i = 0; i < 5; i++) {
            DrawLine(LeftMargin, BottomLine + (StaffGap * i), RightMargin, BottomLine + (StaffGap * i));
        }

        int measures = LineWidth / MeasureWidth;
        for (int i = 1; i <= measures; i++) {
            DrawLine(i * MeasureWidth, TopLine, i * MeasureWidth, BottomLine);
        }
    }


    void DrawNote(Note* note) {
        int topNoteOffset = BottomLine - StaffGap*2;
        int noteX = note->beat * (BeatWidth + BeatPadding) - 0.5f * (BeatWidth + BeatPadding);
        int noteY = topNoteOffset + ((StaffGap/2) * (((TopNote->octave - note->octave) * 7) + TopNote->pitch - note->pitch));
        switch(note->length) {
            case LENGTH::WHOLE:
                DrawCircle(noteX, noteY, BeatWidth / 2);
                break;
            case LENGTH::HALF:
                DrawCircle(noteX, noteY, BeatWidth / 2);
                //lower half, points up
                if (note < MiddleNote) {
                    DrawLine(noteX + BeatWidth / 2, noteY, noteX + BeatWidth / 2, noteY - 15);
                }
                //upper half, points down
                else {
                    DrawLine(noteX - BeatWidth / 2, noteY, noteX - BeatWidth / 2, noteY + 15);
                }
                break;
            case LENGTH::QUARTER:
                FillCircle(noteX, noteY, BeatWidth / 2);
                //lower half, points up
                if (note < MiddleNote) {
                    DrawLine(noteX + BeatWidth / 2, noteY, noteX + BeatWidth / 2, noteY - 15);
                }
                //upper half, points down
                else {
                    DrawLine(noteX - BeatWidth / 2, noteY, noteX - BeatWidth / 2, noteY + 15);
                }
                break;
            }
        if (note->accidental == SHARP) {

        }
  


    }
};

int main() {
    ConsoleMusic cm;
    cm.ConstructConsole(240, 360, 4, 2);
    cm.Start();

    return 0;
}