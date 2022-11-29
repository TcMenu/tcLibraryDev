//
// Created by dave on 14/11/2022.
//

#ifndef TCLIBRARYDEV_UNICODEFONTHANDLER_H
#define TCLIBRARYDEV_UNICODEFONTHANDLER_H

#include <PlatformDetermination.h>
#include <SCCircularBuffer.h>
#include "DrawingPrimitives.h"
#include "GraphicsDeviceRenderer.h"
#include "Utf8TextProcessor.h"
#include "UnicodeFontDefs.h"

namespace tcgfx {
    /**
     * Represents an item that can be drawn using the TcMenu font drawing functions. Regardless of if it is Adafruit
     * or TcUnicode we wrap it in one of these so the drawing code is always the same.
     */
    class GlyphWithBitmapX {
    private:
        const uint8_t* bitmapData = nullptr;
        const UnicodeFontGlyph* glyph = nullptr;
    public:
        /**
         * @return the actual bitmap data with offset already applied
         */
        const uint8_t *getBitmapData() const {
            return bitmapData;
        }

        /**
         * @return the glyph instructions for rendering.
         */
        const UnicodeFontGlyph* getGlyph() const {
            return glyph;
        }

        void setBitmapData(const uint8_t *bm) {
            GlyphWithBitmapX::bitmapData = bm;
        }

        void setGlyph(const UnicodeFontGlyph* g) {
            GlyphWithBitmapX::glyph = g;
        }
    };

    void handleUtf8Drawing(void* userData, uint32_t ch);

    class UnicodeFontHandlerRemove {
    public:
        enum HandlerMode { HANDLER_SIZING_TEXT, HANDLER_DRAWING_TEXT};
    private:
        tccore::Utf8TextProcessor utf8;
        DeviceDrawable* drawable;
        union {
            const UnicodeFont *unicodeFont;
            const GFXfont * adaFont;
        };
        bool fontAdafruit = false;
        Coord currentPosition {};
        HandlerMode handlerMode = HANDLER_DRAWING_TEXT;
        uint16_t xExtentCurrent = 0;
    public:
        explicit UnicodeFontHandlerRemove(DeviceDrawable *drawable, tccore::UnicodeEncodingMode mode): utf8(handleUtf8Drawing, this, mode), drawable(drawable), unicodeFont(nullptr), currentPosition() {}
        /**
         * Sets the font to be a TcUnicode font
         * @param font a tcUnicode font
         */
        void setFont(const UnicodeFont *font) { unicodeFont = font; fontAdafruit = false; }
        /**
         * sets the font to be an Adafruit font
         * @param font an adafruit font
         */
        void setFont(const GFXfont *font) { adaFont = font; fontAdafruit = true; }

        /**
         * Sets the font based on the magnification, 0 value means the font is a unicode font,
         * any other value is adafruit. The usual way is to set magnification to 0 for unicode,
         * otherwise set it to 1 or more for adafruit.
         * @param font a void font pointer to be decided using mag
         * @param mag when -1=Unicode, otherwise Adafruit
         */
        void setFontFromMag(const void* font, int mag);

        /**
         * Set the position at which the next item will be printed.
         * @param where
         */
        void setCursor(const Coord& where) {
            currentPosition = where;
        }

        /**
         * Print a UTF8 string using the selected font, if characters are missing they are skipped
         * @param text the UTF8 text to print
         */
        void printUtf8(const char *text);

        /**
         * Prints a unicode character using the current font
         * @param unicodeChar the character to print.
         */
        void writeUnicode(uint32_t unicodeText);

        /**
         * Get the extents of the text provided in UTF8
         * @param text the text to get the UTF8 version of
         * @param baseline pointer to in for¬ the baseline of the text (optional)
         * @return the x and y extent of the text
         */
        Coord textExtents(const char *text, int *baseline);

        /**
         * Get the extent of a single unicode character
         * @param theChar the unicode character to get the extent of
         * @return the x and y extent of the character
         */
        Coord textExtent(uint32_t theChar);

        /**
         * Get the absolute baseline of the font, this is the total height including any parts considered below
         * the yheight.
         * @return the baseline
         */
        int getBaseline();

        /**
         * Can be used to implement the print interface, the UTF8 implementation for this library is completely
         * asynchronous and keeps internal state, so it will wait until enouogh characters arrive to actually print
         * something.
         * @param data a byte of data in utf8
         */
        void write(uint8_t data);

        void internalHandleUnicodeFont(uint32_t ch);
    private:
        bool findCharInFont(uint32_t ch, GlyphWithBitmapX& glyphBitmap) const;
        int getYAdvance() const { return (int32_t)(fontAdafruit ? adaFont->yAdvance : unicodeFont->yAdvance); }
    };
}

#endif //TCLIBRARYDEV_UNICODEFONTHANDLER_H
