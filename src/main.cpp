#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ticker.h>
#include <Wire.h>
Adafruit_SSD1306 display(128, 64, &Wire);
Ticker ticker;

void swap(int *p0, int *p1);
void drawChess(int x0, int y0, bool *p);
void drawMap(long n);
void pointer();
void drawPointer(int x0, int y0);
void swapchess();

bool chesspixels[7][25]{
    {1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1,
     0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0}};

int chessmap[10][9]{
    {-1, -2, -3, -4, -5, -4, -3, -2, -1}, {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, -6, 0, 0, 0, 0, 0, -6, 0},        {-7, 0, -7, 0, -7, 0, -7, 0, -7},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},          {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {7, 0, 7, 0, 7, 0, 7, 0, 7},          {0, 6, 0, 0, 0, 0, 0, 6, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},          {1, 2, 3, 4, 5, 4, 3, 2, 1}};

long n = 0;
long time1 = 0;
long time2 = 0;
int selected_x = 0;
int selected_y = 0;
bool selected_lock = 0;
int pointer_x = 0;
int pointer_y = 0;

void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    Serial.begin(9600);
    pinMode(D3, INPUT_PULLUP);
    pinMode(D5, INPUT_PULLUP);
    pinMode(D7, INPUT_PULLUP);
    ticker.attach(0.08, pointer);
}

void loop() {
    // compute fps
    long dtime = micros() - time1;
    time1 = micros();
    double fps = 1000000.0/dtime;
    Serial.println(fps);
    //finish compute
    n++;
    drawPointer(pointer_x, pointer_y);
    swapchess();
    drawMap(n);
    display.display();
    display.clearDisplay();
}

void swap(int *p0, int *p1) {
    int i = *p0;
    *p0 = *p1;
    *p1 = i;
}

void drawChess(int x0, int y0, bool *p) {
    int x = 2 + 7 * x0;
    int y = 2 + 6 * y0;
    for (int i = 0; i < 25; i++) {
        if (*(p + i)) {
            display.drawPixel(x, y, WHITE);
        }
        x++;
        if ((i + 1) % 5 == 0) {
            x = 2 + 7 * x0;
            y = y + 1;
        }
    }
}

void drawMap(long n) {
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 9; x++) {
            int id = abs(chessmap[y][x]);
            bool *p = &chesspixels[id - 1][0];
            if (id) {
                if ((!(n % 100)) && (chessmap[y][x] < 0)) {
                    break;
                }
                drawChess(x, y, p);
            }
        }
    }
}

void pointer() {
    if (!digitalRead(D5) && (pointer_y * pointer_x < 72)) {
        pointer_x++;
    }
    if (!digitalRead(D3) && ((pointer_y + pointer_x) > 0)) {
        pointer_x--;
    }
    if (pointer_x == 9) {
        pointer_x = 0;
        pointer_y++;
    }
    if (pointer_x == -1) {
        pointer_x = 8;
        pointer_y--;
    }
}

void drawPointer(int x0, int y0) {
    int x = 1 + 7 * x0;
    int y = 1 + 6 * y0;
    display.drawPixel(x + 0, y + 0, WHITE);
    display.drawPixel(x + 1, y + 0, WHITE);
    display.drawPixel(x + 5, y + 0, WHITE);
    display.drawPixel(x + 6, y + 0, WHITE);
    display.drawPixel(x + 0, y + 1, WHITE);
    display.drawPixel(x + 6, y + 1, WHITE);
    display.drawPixel(x + 0, y + 5, WHITE);
    display.drawPixel(x + 6, y + 5, WHITE);
    display.drawPixel(x + 0, y + 6, WHITE);
    display.drawPixel(x + 1, y + 6, WHITE);
    display.drawPixel(x + 5, y + 6, WHITE);
    display.drawPixel(x + 6, y + 6, WHITE);
}

void swapchess(){
    if(!digitalRead(D7) && ((millis() - time2) > 1000) && selected_lock == 0){
        time2 = millis();
        selected_lock = 1;
        selected_x = pointer_x;
        selected_y = pointer_y;
    }
    if(!digitalRead(D7) && ((millis() - time2)>1000) && selected_lock == 1){
        time2 = millis();
        selected_lock = 0;
        swap(&chessmap[selected_y][selected_x],
             &chessmap[pointer_y][pointer_x]);
        if (chessmap[selected_y][selected_x] != 0) {
            chessmap[selected_y][selected_x] = 0;
        }
    }
}