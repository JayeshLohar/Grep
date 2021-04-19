typedef struct regex_t{
    unsigned char  type;
    union{
        unsigned char  ch;
        unsigned char* ccl;
    } u;
} regex_t;

typedef struct regex_t* re_t;
re_t re_compile(const char* pattern);
int re_matchp(re_t pattern, const char* text, int* matchlength);
int re_match( const char* text, const char* pattern, int* matchlength);