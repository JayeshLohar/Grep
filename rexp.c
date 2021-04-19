#include <stdio.h>
#include <ctype.h>
#include "rexp.h"

#define MAX_REGEXP_OBJECTS      30
#define MAX_CHAR_CLASS_LEN      40

enum { UNUSED, DOT, BEGIN, END, QUESTIONMARK, STAR, PLUS, CHAR, CHAR_CLASS, INV_CHAR_CLASS, DIGIT, NOT_DIGIT, ALPHA, NOT_ALPHA, WHITESPACE, NOT_WHITESPACE, /* BRANCH */ };

static int matchpattern( regex_t* pattern, const char* text, int* matchlength);
static int matchcharclass( char c, const char* str);
static int matchstar( regex_t p, regex_t* pattern, const char* text, int* matchlength);
static int matchplus( regex_t p, regex_t* pattern, const char* text, int* matchlength);
static int matchone( regex_t p, char c);
static int matchdigit(char c);
static int matchalpha(char c);
static int matchwhitespace(char c);
static int matchmetachar(char c, const char* str);
static int matchrange(char c, const char* str);
static int ismetachar(char c);

int re_match(  const char* text, const char* pattern, int* matchlength){
    return re_matchp(re_compile(pattern), text, matchlength);
}

int re_matchp(re_t pattern, const char* text, int* matchlength){
    *matchlength = 0;
    if (pattern != 0){
        if (pattern[0].type == BEGIN)
            return ((matchpattern(&pattern[1], text, matchlength)) ? 0 : -1);
        else{
            int idx = -1;
            do{
                idx += 1;
                if (matchpattern(pattern, text, matchlength)) {
                    if (text[0] == '\0')
                    return -1;
                    return idx;
                }
            } while (*text++ != '\0');
        }
    }
    return -1;
}

re_t re_compile(const char* pattern){
    static regex_t re_compiled[MAX_REGEXP_OBJECTS];
    static unsigned char ccl_buf[MAX_CHAR_CLASS_LEN];
    int ccl_bufidx = 1;

    char c;
    int i = 0, j = 0 ;

    while (pattern[i] != '\0' && (j+1 < MAX_REGEXP_OBJECTS)) {
        c = pattern[i];
        switch (c){
            case '^':
                re_compiled[j].type = BEGIN ; break ;
            case '$':
                re_compiled[j].type = END ; break ;
            case '.':
                re_compiled[j].type = DOT ; break ;
            case '*':
                re_compiled[j].type = STAR ; break ;
            case '+':
                re_compiled[j].type = PLUS ; break ;
            case '?':
                re_compiled[j].type = QUESTIONMARK ; break ;
            case '\\':
                if (pattern[i+1] != '\0'){
                    i += 1;
                    switch (pattern[i]){
                        case 'd':
                            re_compiled[j].type = DIGIT ; break ;
                        case 'D':
                            re_compiled[j].type = NOT_DIGIT ; break ;
                        case 'w':
                            re_compiled[j].type = ALPHA ; break ;
                        case 'W':
                            re_compiled[j].type = NOT_ALPHA; break ;
                        case 's':
                            re_compiled[j].type = WHITESPACE ; break ;
                        case 'S':
                            re_compiled[j].type = NOT_WHITESPACE ; break ;
                        default:
                            re_compiled[j].type = CHAR;
                            re_compiled[j].u.ch = pattern[i];
                            break;
                    }
                }
                break ;

            case '[':{
                int buf_begin = ccl_bufidx;
                if (pattern[i+1] == '^'){
                    re_compiled[j].type = INV_CHAR_CLASS;
                    i += 1;
                    if (pattern[i+1] == 0){
                        return 0;
                    }
                }
                else{
                re_compiled[j].type = CHAR_CLASS;
                }

                while ( (pattern[++i] != ']') && (pattern[i]   != '\0') ){
                    if (pattern[i] == '\\'){
                        if (ccl_bufidx >= MAX_CHAR_CLASS_LEN - 1){
                            return 0;
                        }
                        if (pattern[i+1] == 0){
                            return 0;
                        }
                        ccl_buf[ccl_bufidx++] = pattern[i++];
                    }
                    else if (ccl_bufidx >= MAX_CHAR_CLASS_LEN){
                        return 0;
                    }
                    ccl_buf[ccl_bufidx++] = pattern[i];
                }
                if (ccl_bufidx >= MAX_CHAR_CLASS_LEN){
                    return 0;
                }

                ccl_buf[ccl_bufidx++] = 0;
                re_compiled[j].u.ccl = &ccl_buf[buf_begin];
            } break;

            default:{
                re_compiled[j].type = CHAR;
                re_compiled[j].u.ch = c;
            } break;
        }
        if (pattern[i] == 0){
            return 0;
        }

        i++ ;
        j++ ;
    }
    re_compiled[j].type = UNUSED;
    return (re_t) re_compiled;
}

void re_print(regex_t* pattern){
    const char* types[] = { "UNUSED", "DOT", "BEGIN", "END", "QUESTIONMARK", "STAR", "PLUS", "CHAR", "CHAR_CLASS", "INV_CHAR_CLASS", "DIGIT", "NOT_DIGIT", "ALPHA", "NOT_ALPHA", "WHITESPACE", "NOT_WHITESPACE", "BRANCH" };

    int i, j ;
    char c;
    for (i = 0; i < MAX_REGEXP_OBJECTS; ++i){
        if (pattern[i].type == UNUSED){
            break;
        }

        printf("type: %s", types[pattern[i].type]);
        if (pattern[i].type == CHAR_CLASS || pattern[i].type == INV_CHAR_CLASS){
            printf(" [");
            for (j = 0; j < MAX_CHAR_CLASS_LEN; ++j){
                c = pattern[i].u.ccl[j];
                if ((c == '\0') || (c == ']')){
                    break;
                }
                printf("%c", c);
            }
            printf("]");
        }
        else if (pattern[i].type == CHAR){
            printf(" '%c'", pattern[i].u.ch);
        }
        printf("\n");
    }
}

static int matchdigit(char c){
  return isdigit(c);
}

static int matchalpha(char c)
{
  return isalpha(c);
}

static int matchwhitespace(char c){
  return isspace(c);
}

static int matchalphanum(char c){
  return ((c == '_') || matchalpha(c) || matchdigit(c));
}

static int matchrange(char c, const char* str){
    return ( (c != '-') && (str[0] != '\0') && (str[0] != '-') && (str[1] == '-') && (str[2] != '\0') && ( (c >= str[0]) && (c <= str[2]) ) );
}

static int ismetachar(char c){
  return ((c == 's') || (c == 'S') || (c == 'w') || (c == 'W') || (c == 'd') || (c == 'D'));
}

static int matchmetachar(char c, const char* str){
    switch (str[0]){
        case 'd': return  matchdigit(c);
        case 'D': return !matchdigit(c);
        case 'w': return  matchalphanum(c);
        case 'W': return !matchalphanum(c);
        case 's': return  matchwhitespace(c);
        case 'S': return !matchwhitespace(c);
        default:  return (c == str[0]);
    }
}

static int matchcharclass(char c, const char* str){
    do{
        if (matchrange(c, str)){
            return 1;
        }
        else if (str[0] == '\\'){
            str += 1;
            if (matchmetachar(c, str)){
                return 1;
            }
            else if ((c == str[0]) && !ismetachar(c)){
                return 1;
            }
        }
        else if (c == str[0]){
            if (c == '-'){
                return ((str[-1] == '\0') || (str[1] == '\0'));
            }
            else{
                return 1;
            }
        }
    }while (*str++ != '\0');
    return 0;
}

static int matchone(regex_t p, char c){
    switch (p.type){
        case CHAR_CLASS:     return  matchcharclass(c, (const char*)p.u.ccl);
        case INV_CHAR_CLASS: return !matchcharclass(c, (const char*)p.u.ccl);
        case DIGIT:          return  matchdigit(c);
        case NOT_DIGIT:      return !matchdigit(c);
        case ALPHA:          return  matchalphanum(c);
        case NOT_ALPHA:      return !matchalphanum(c);
        case WHITESPACE:     return  matchwhitespace(c);
        case NOT_WHITESPACE: return !matchwhitespace(c);
        default:             return  (p.u.ch == c);
    }
}

static int matchstar(regex_t p, regex_t* pattern, const char* text, int* matchlength){
    int prelen = *matchlength;
    const char* prepoint = text;
    while ((text[0] != '\0') && matchone(p, *text)){
        text++;
        (*matchlength)++;
    }
    while (text >= prepoint){
        if (matchpattern(pattern, text--, matchlength))
            return 1;
        (*matchlength)--;
    }

    *matchlength = prelen;
    return 0;
}

static int matchplus(regex_t p, regex_t* pattern, const char* text, int* matchlength){
    const char* prepoint = text;
    while ((text[0] != '\0') && matchone(p, *text)){
        text++;
        (*matchlength)++;
    }
    while (text > prepoint){
        if (matchpattern(pattern, text--, matchlength))
            return 1;
        (*matchlength)--;
    }
    return 0;
}

static int matchquestion(regex_t p, regex_t* pattern, const char* text, int* matchlength){
    if (p.type == UNUSED)
        return 1;
    if (matchpattern(pattern, text, matchlength))
        return 1;
    if (*text && matchone(p, *text++)){
        if (matchpattern(pattern, text, matchlength)){
            (*matchlength)++;
            return 1;
        }
    }
    return 0;
}

static int matchpattern(regex_t* pattern, const char* text, int* matchlength){
    int pre = *matchlength;
    do{
        if ((pattern[0].type == UNUSED) || (pattern[1].type == QUESTIONMARK))
            return matchquestion(pattern[0], &pattern[2], text, matchlength);

        else if (pattern[1].type == STAR)
            return matchstar(pattern[0], &pattern[2], text, matchlength);

        else if (pattern[1].type == PLUS)
            return matchplus(pattern[0], &pattern[2], text, matchlength);

        else if ((pattern[0].type == END) && pattern[1].type == UNUSED)
            return (text[0] == '\0');

        (*matchlength)++;
    }while ((text[0] != '\0') && matchone(*pattern++, *text++));

    *matchlength = pre;
    return 0;
}
