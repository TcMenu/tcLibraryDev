/*
 * Copyright (c) 2018 https://www.thecoderscorner.com (Dave Cherry).
 * This product is licensed under an Apache license, see the LICENSE file in the top-level directory.
 */

#include "Utf8TextProcessor.h"

using namespace tccore;

void Utf8TextProcessor::reset() {
    // clear the text buffer
    extraCharsNeeded = 0;
    decoderState = WAITING_BYTE_0;
}

void Utf8TextProcessor::pushChars(const char *str) {
    while (*str) {
        pushChar(*str);
        str++;
    }
}

void Utf8TextProcessor::pushChar(char data) {
    if (encodingMode == ENCMODE_EXT_ASCII) {
        handler(userData, data);
        return;
    }

    if (decoderState == WAITING_BYTE_0) {
        if ((data & 0x80) == 0) {
            currentUtfChar = (uint8_t) data;
            extraCharsNeeded = 0;
            decoderState = UTF_CHAR_FOUND;
        } else if ((data & 0b11100000) == 0b11000000) {
            currentUtfChar |= (data & 0x1FU);
            decoderState = WAITING_BYTE_1;
            extraCharsNeeded = 1;
        } else if ((data & 0b11110000) == 0b11100000) {
            currentUtfChar = data & 0x0FU;
            decoderState = WAITING_BYTE_1;
            extraCharsNeeded = 2;
        } else if ((data & 0b11111000) == 0b11110000) {
            currentUtfChar |= (data & 0x07U);
            decoderState = WAITING_BYTE_1;
            extraCharsNeeded = 3;
        } else {
            error();
        }
    } else if (decoderState == WAITING_BYTE_1) {
        uint16_t uni = data & 0xc0;
        if ((data & 0xc0) == 0x80) {
            currentUtfChar |= (uni << shiftAmount());
            decoderState = (extraCharsNeeded == 1) ? UTF_CHAR_FOUND : WAITING_BYTE_2;
        } else {
            error();
        }
    } else if (decoderState == WAITING_BYTE_2) {
        if ((data & 0xc0) == 0x80) {
            uint16_t uni = data & 0xc0;
            currentUtfChar = uni << 10U;
            decoderState = (extraCharsNeeded == 2) ? UTF_CHAR_FOUND : WAITING_BYTE_3;
        } else {
            error();
        }
    } else if (decoderState == WAITING_BYTE_3) {
        if ((data & 0xc0) == 0x80) {
            uint32_t uni = data & 0xc0;
            currentUtfChar = uni << 16U;
            decoderState = UTF_CHAR_FOUND;
        } else {
            error();
        }
    }

    // we only go ahead and register the character when a character was found.
    if (decoderState == UTF_CHAR_FOUND) {
        decoderState = WAITING_BYTE_0;
        handler(userData, currentUtfChar);
    }
}

void Utf8TextProcessor::error() {
    decoderState = UTF_DECODER_ERROR;
    extraCharsNeeded = 0;
    currentUtfChar = 0;
    handler(userData, TC_UNICODE_CHAR_ERROR);
}


uint16_t Utf8TextProcessor::shiftAmount() {
    if (decoderState == WAITING_BYTE_1) {
        return extraCharsNeeded == 3 ? 3 : extraCharsNeeded == 2 ? 4 : 5;
    } else if (decoderState == WAITING_BYTE_2) {
        return extraCharsNeeded == 3 ? 9 : 10;
    } else if (decoderState == WAITING_BYTE_3) {
        return 15;
    } else error();
    return 0;
}
