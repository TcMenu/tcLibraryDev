#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C gfx(U8G2_R0, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE);

void setup() {
    Serial.begin(115200);
    Serial.println("U8G2 kanji print test");
    Wire.begin();
    gfx.begin();
    gfx.setFont(u8g2_font_wqy12_t_gb2312b);

    // get the extents of the text
    auto textToPrint = "1分钟abcde";
    auto extraHeight = gfx.getMaxCharHeight();
    auto baseline = gfx.getFontDescent();
    auto xLen = gfx.getStrWidth(textToPrint);
    auto xLenUtf8 = gfx.getUTF8Width(textToPrint);
    gfx.drawUTF8(0, 20, textToPrint);

    // show where the text extent said the text ends
    gfx.drawLine(xLen, 0, xLen, 10);
    gfx.drawLine(xLenUtf8, 0, xLenUtf8, 10);
    gfx.sendBuffer();

    Serial.print("Text      = "); Serial.println(textToPrint);
    Serial.print("MaxHeight = "); Serial.println(extraHeight);
    Serial.print("Baseline  = "); Serial.println(baseline);
    Serial.print("xLength   = "); Serial.println(xLen);
    Serial.print("xLengthUTF= "); Serial.println(xLenUtf8);
}

void loop() {

}