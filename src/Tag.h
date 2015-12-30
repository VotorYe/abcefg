#ifndef __TAG_H__
#define __TAG_H__
struct TAG {
        const int
        CREATE = 151, VIEW = 152, AS = 153, OUTPUT = 154,
        SELECT = 155, FROM = 156, EXTRACT = 157, REGEX = 158,
        ON = 159, RETURN = 160, GROUP = 161, AND = 162,
        TOKEN = 163, PATTERN = 164, // Keyword

        NUM = 201,  ID = 202, // 1 . , id

        LEFTPARENTHESIS = 301, RIGHTPARENTHESIS = 302, // ()
        LEFTBRACE = 303, RIGHTBRACE = 304,  // {}
        LESS = 305, GREATER = 306, //<>

        DOT = 401, COMMA = 402, SEMICOLON = 403, // . , ;

        REG = 501,
        END = 10;
} Tag;
#endif
