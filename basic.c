#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<ctype.h>
#include "basic.h"
#include "color.h"
#include "grep.h"
#include "rexp.h"
#include<limits.h>

void lower( char *str ){
    int i = 0 ;
    while( str[i] != '\0' ){
        if( str[i] >= 'A' && str[i] <= 'Z' )
            str[i] += 32 ;
        i++ ;
    }
    return ;
}

int str_str( char *p, char *q ){
    int i = 0, j = 0 ;
    while( p[i] != '\0' && q[j] != '\0' && p[i] != '\n' && q[j] != '\n'){
        if( p[i] == q[j] )
            j++ ;
        else
            j = 0 ;
        i++ ;
    }
    if( q[j] == '\0' || q[j] == '\n' )
        return i -j ;
    else
        return -1 ;
}

int str_case_str( char *str, char *ch ){
    char *p = (char*)malloc( strlen( str));
    char *q = (char*)malloc( strlen( ch));
    strcpy( p, str );
    strcpy( q, ch );
    lower( p );
    lower( q );
    int i = str_str( p, q );
    free(p);
    return i ;
}

int str_str_word( char *p, char *q){
    int i = 0, j = 0 ;
    while( p[i] != '\0' && q[j] != '\0' && p[i] != '\n' && q[j] != '\n' ){
        if( p[i] == q[j] )
            j++ ;
        else{
            j = 0 ;
            while( isalnum( p[i]) || p[i] == '_' )
                i++ ;
            if( p[i] == '\0' || p[i] == '\n' )
                return -1 ;
        }
        i++ ;
        if( q[j] == '\0' || q[j] == '\n' ){
            if( isalnum( p[i]) || p[i] == '_' )
                j = 0 ;
            else
                return i -j ;
        }
    }
    return -1 ;
}

int str_case_str_word( char *str, char *ch ){
    char *p = (char*)malloc( strlen( str));
    char *q = (char*)malloc( strlen( ch));
    strcpy( p, str );
    strcpy( q, ch );
    lower( p );
    lower( q );
    int i = str_str_word( p, q );
    free(p);
    return i ;
}

int str_str_last( char *str, char *ch, int i, int w ){
    int j , k ;
    j = strlen(str) -1 ;
    k = strlen(ch) -1 ;
    char *p = str, *q = ch ;
    while( p[j] == '\n' || p[j] == '\0' || p[j] == 13 )
        j-- ;
    while( q[k] == '\n' || q[k] == '\0' )
        k-- ;
    if( k > j )
        return -1 ;
    p = p + j - k ;
    j = j -k;
    k = my_strstr( p, q, i, w, 0 );
    if( k == 0 ){
        if( w && j > 0 && str[j-1] != ' ' )
            return -1;
        return j ;
    }
    return -1 ;
}

int str_str_x( char *p, char *q ){
    int i = 0 ;
    while( p[i] != '\0' && q[i] != '\0' && p[i] != '\n' && q[i] != '\n'){
        if( p[i] != q[i] )
            return -1 ;
        i++ ;
    }
    if( ( p[i] == '\0' || p[i] == '\n' ) && ( q[i] =='\n' || q[i] == '\0' ) )
        return 0 ;
    else
        return -1 ;
}

int str_case_str_x( char *str, char *ch ){
    char *p = (char*)malloc( strlen( str));
    char *q = (char*)malloc( strlen( ch));
    strcpy( p, str );
    strcpy( q, ch );
    lower( p );
    lower( q );
    int i = str_str_x( p, q );
    free(p);
    free(q);
    return i ;
}

int my_strstr( char *str, char *ch, int i, int w, int x ){
    int j ;
    if(x)
        j = ( i == 0 ) ? str_str_x( str, ch) : str_case_str_x( str, ch);
    else if(i)
        j = ( w == 0 ) ? str_case_str( str, ch) : str_case_str_word( str, ch);
    else
        j = ( w == 0 ) ? str_str( str, ch) : str_str_word( str, ch );
    return j ;
}

void print_patt( char *str, char *ch, int i, int w ){
    int k, j, l1 = strlen( ch );
    char *p = str ;
    k = my_strstr( p, ch, i, w, 0 ) ;
    while( k != -1 ){
        for( j = 0 ; j < k ; j++ ){
            printf("%c", p[0]);
            p++ ;
        }
        purple();
        for( j = 0 ; j < l1; j++ ){
            printf("%c", p[0]);
            p++ ;
        }
        reset();
        k = my_strstr( p, ch, i, w, 0 ) ;
    }
    printf("%s", p);
    return ;
}

void my_print( char *str, char *ch, int i, int w, int x, int v ){
    if(v)
        printf("%s", str);
    else if(x){
        purple();
        printf("%s", str);
        reset();
    }
    else
        print_patt( str, ch, i, w );
}

int rexp_word( char *str, char *ch, int *l ){
    int len, k, index = 0 ;
    char *p = str ;
    k = re_match( str, ch, &len );
    while( k != -1 ){
        index += k ;
        p = p + k + len ;
        *l = len ;
        if( index != 0 ){
            if( !isalnum( str[index-1]) && str[index-1] != '_' ){
                if( !isalnum( p[0]) && p[0] != '_' )
                    return index ;
            }
        }
        else{
            if( !isalnum( p[0]) && p[0] != '_' )
                    return index ;
        }
        index = index + len ;
        k = re_match( p, ch, &len );
    }
    return -1 ;
}

void print_exp( char *str, char *ch, int w ){
    int len, k, j ;
    char *p = str ;
    k = ( w == 0 ) ? re_match( str, ch, &len ) : rexp_word( str, ch, &len );
    while( k != -1 ){
        for( j = 0 ; j < k ; j++ ){
            printf("%c", p[0]);
            p++ ;
        }
        purple();
        for( j = 0 ; j < len; j++ ){
            printf("%c", p[0]);
            p++ ;
        }
        reset();
        k = ( w == 0 ) ? re_match( p, ch, &len ) : rexp_word( p, ch, &len );
    }
    printf("%s", p);
}

int my_getline(char **line, int *len, FILE *fp ){
    if( !len || !fp )
        return -1 ;

    int i = -1 ;
    *len = 0 ;
    char chunk[128] ;

    while( fgets( chunk, 128, fp ) != NULL ){
        i++ ;
        if( i == 0 ){
            if( (*line = (char*)malloc( sizeof(char) *128 )) == NULL )
                return -1 ;
            strcpy( *line, chunk );
        }
        else{
            (*line) = (char*) realloc( *line, (i+2) * 128) ;
            strcat( *line, chunk );
        }
        *len = strlen( *line );
        if( (*line)[ *len -1] == '\n' )
            return i ;
    }
    if( i != -1 ){
        strcat( *line, "\n");
        *len = strlen( *line);
    }
    return i ;
}

void init( patt_line *f ){
    f->l_no = 0 ;
    f->line = NULL ;
    return ;
}

void add_line( patt_line *f, char *l ){
    int i = f->l_no ;
    i++ ;
    if( i == 1 )
        f->line = (char**)malloc(sizeof(char*));
    else
        f->line = (char**)realloc( f->line, sizeof(char*) * i );
    if( f->line == NULL )
        return ;
    f->line[f->l_no] = l ;
    f->l_no++ ;
    return ;
}

void read_file( char *fname, patt_line *f ){
    FILE *fp = fopen( fname, "r");
    char *line ;
    int len ;

    if( fp == NULL ){
        printf("'%s' File does not Exit..!! \n", fname );
        return ;
    }
    while( my_getline( &line, &len, fp ) != -1 ){
        strcat( line, "\0");
        add_line( f, line );
    }
    fclose(fp);
    return ;
}

int isfile( char *fname ){
    DIR *d = opendir( fname);
    if( d ){
        closedir(d);
        return 2 ;
    }
    FILE *fp = fopen( fname, "r");
    if( fp ){
        fclose(fp);
        return 1 ;
    }
    return 0 ;
}