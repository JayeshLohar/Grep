int str_str(char *str, char *ch);
int str_str_word(char *str, char *ch);
int str_case_str( char *str, char *ch );
int str_case_str_word( char *str, char *ch );
int str_str_x( char *p, char *q );
int str_case_str_x( char *str, char *ch );
int str_str_last( char *str, char *ch, int i, int w );

int my_strstr( char *str, char *ch, int i, int w, int x );
void print_patt( char *str, char *ch, int i, int w );
void my_print( char *str, char *ch, int i, int w, int x, int v );
int rexp_word( char *str, char *ch, int *l );
void print_exp( char *str, char *ch, int w );

int my_getline(char **line, int *len, FILE *fp );

typedef struct patt_line{
    char **line ;
    int l_no ;
}patt_line ;

void init( patt_line *f );
void add_line( patt_line *f, char *l );
void read_file( char *fname, patt_line *f );
void lower( char *str );

int isfile( char *fname );