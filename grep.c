#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<dirent.h>
#include "grep.h"
#include "basic.h"
#include "color.h"
#include "rexp.h"

/*
 * mode : 0 = print only match line, 1 = line num, 2 = byte offset, 3 = both line num & byte offset, 4 = number of match line
 * i : Case insensitive
 * v : Invert match
 * w : Match whole word
 * x : Match whole line
 * h : Print filename
 * m : Print upto m matches
 * s : 0 = any match, 1 = start, 2 = end 
 * E : Expression
 */
void grep( char *fname, char *pattern, int mode, int i, int v, int w, int x, int h, int m, int s, int E ){
    if( E )
        s = 0 ;

    int line_num = 0 , byte_offset = 0, no = 0, len = 0, j, match_length ;

    FILE *fp = fopen( fname, "r");
    if( fp == NULL ){
        printf("'%s' File does not Exit..!!", fname );
        return ;
    }

    char *line ;

    while( my_getline( &line, &len, fp ) != -1 && no < m ){
        line_num++ ;
        if( E )
            j = ( w == 0 ) ? re_match( line, pattern, &match_length) : rexp_word( line, pattern, &match_length );
        else
            j = ( s == 2 ) ? str_str_last( line, pattern, i, w ) : my_strstr( line, pattern, i, w, x ) ;

        if( s == 1 && j != 0 )    
            j = -1 ;
        if( ( !v && j != -1 ) || ( v && j == -1 ) ){
            no++ ;
            if( mode != 4 ){
                if( h == 1 ){
                    yellow();
                    printf("%s : ", fname );
                    reset();
                }
                if( mode == 1 || mode == 3 ){
                    cyan();
                    printf("%d. ", line_num );
                    reset();
                }
                if( mode == 2 || mode == 3 ){
                    cyan();
                    printf("%d. ", byte_offset );
                    reset();
                }
                if( E )
                    print_exp( line, pattern, w );
                else
                    my_print( line, pattern, i, w, x, v );
            }
        }
        byte_offset += len ;
    }
    if( mode == 4 ){
        if( h == 1 ){
            yellow();
            printf("%s : ", fname );
            reset();
        }
        cyan();
        printf("%d \n", no );
        reset();
    }
    fclose( fp );
    free( line );
    return ;
}

void grep_o( char *fname, char *pattern, int mode, int i, int w, int h, int m, int s, int E ){
    if( E )
        s = 0 ;

    int line_num = 0 , byte_offset = 0, no = 0, len, a = 0 ;

    FILE *fp = fopen( fname, "r");
    if( fp == NULL ){
        printf("'%s' File does not Exit..!!", fname );
        return ;
    }

    char *line , *p ;
    int len_pattern = strlen( pattern ), k , j = 0 ;

    while( my_getline( &line, &len, fp ) != -1 && no < m ){
        line_num++ ;
        p = line ;
        a = 0 ;
        if( E )
            k = ( w == 0 ) ? re_match( line, pattern, &len_pattern) : rexp_word( line, pattern, &len_pattern ) ;
        else
            k = ( s == 2 ) ? str_str_last( line, pattern, i, w ) : my_strstr( line, pattern, i, w, 0 ) ;
        if( s == 1 && k != 0 )    
            k = -1 ;

        while( k != -1 && no < m && a == 0 ){
            no++ ;
            if( s != 0 )
                a++ ;
            p = p + k ;
            if( mode != 4 ){
                if( h == 1 ){
                    yellow();
                    printf("%s : ", fname );
                    reset();
                }
                if( mode == 1 || mode == 3 ){
                    cyan();
                    printf("%d. ", line_num );
                    reset();
                }
                if( mode == 2 || mode == 3 ){
                    cyan();
                    printf("%d. ", byte_offset + k );
                    reset();
                }
                purple();
                for( j = 0 ; j < len_pattern ; j++ ){
                    printf("%c", *p );
                    p++ ;
                }
                printf("\n");
                reset();
            }
            else
                for( j = 0 ; j < len_pattern ; j++ )
                    p++ ;
            if( E )
                k = ( w == 0 ) ? re_match( p, pattern, &len_pattern) : rexp_word( p, pattern, &len_pattern ) ;
            else
                k = my_strstr( p, pattern, i, w, 0 );
        }        
        byte_offset += len ;
    }
    if( mode == 4 ){
        if( h == 1 ){
            yellow();
            printf("%s : ", fname );
            reset();
        }
        cyan();
        printf("%d \n", no );
        reset();
    }

    fclose(fp);
    free( line );
    return ;
}

void grep_l( char *fname, char *pattern, int l, int i, int w, int x, int s, int E ){
    if( E )
        s = 0 ;

    int line_num = 0 , no = 0, len = 0, j, match_length ;

    FILE *fp = fopen( fname, "r");
    if( fp == NULL ){
        printf("'%s' File does not Exit..!!", fname );
        return ;
    }
    char *line ;

    while( my_getline( &line, &len, fp ) != -1 && no == 0 ){
        line_num++ ;
        if( E )
            j = ( w == 0 ) ? re_match( line, pattern, &match_length) : rexp_word( line, pattern, &match_length );
        else
            j = ( s == 2 ) ? str_str_last( line, pattern, i, w ) : my_strstr( line, pattern, i, w, x ) ;
        if( s == 1 && j != 0 )    
            j = -1 ;
        if( j != -1 )
            no++ ;
    }
    if( ( l == 1 && no ) || ( l == 2 && !no ) ){
        yellow();
        printf("%s \n", fname );
        reset();
    }
    fclose( fp );
    free( line );
    return ;
}

void grep_directory( char *dname, char *pattern, int mode, int i, int v, int w, int x, int h, int m, int l, int o, int s, int E ){

    DIR *dir;
    dir = opendir( dname ) ;
    struct dirent *de;
    if( !dir )
        return ;

    int len = strlen( dname );

    char *dir_name = (char*)malloc( sizeof(char) * len ) ;
    char *file_name = (char*)malloc( sizeof(char) * len ) ;
    if( !dir_name || !file_name )
        return ;

    while( ( de = readdir(dir) ) ){
        if( de->d_type == DT_DIR ){
            if(strcmp( de->d_name, ".") == 0 || strcmp( de->d_name, "..") == 0 );
            else{
                strcpy( dir_name, dname ) ;
                strcat( dir_name, "/");
                strcat( dir_name, de->d_name);
                grep_directory( dir_name, pattern, mode, i, v, w, x, h, m, l, o, s, E );
            }
        }
        else if( de->d_type == DT_REG ){
            strcpy( file_name, dname ) ;
            strcat( file_name, "/");
            strcat( file_name, de->d_name);
            if( l )
                grep_l( file_name, pattern, l, i, w, x, s, E );
            else if( o )
                grep_o( file_name, pattern, mode, i, w, h, m, s, E );
            else
                grep( file_name, pattern, mode, i, v, w, x, h, m, s, E );
        }
    }
    closedir(dir);
}

void grep_f( char *fname1, char *fname2, int mode, int i, int v, int w, int x, int h, int m ){

    patt_line f1, f2 ;
    init( &f1 );
    init( &f2 );
    read_file( fname1, &f1 );
    read_file( fname2, &f2 );
    
    int line_num = 0 , byte_offset = 0, no = 0, len = 0, j, k ;
    char *q ;

    while( line_num < f1.l_no && no < m ){
        j = -1 ;
        k = 0 ;
        while( k < f2.l_no && j == -1 ){
            q = f2.line[k] ;
            j =  my_strstr( f1.line[line_num], f2.line[k], i, w, x ) ;
            k++ ;
        }
        if( ( !v && j != -1 ) || ( v && j == -1 ) ){
            no++ ;
            if( mode != 4 ){
                if( h == 1 ){
                    yellow();
                    printf("%s : ", fname1 );
                    reset();
                }
                if( mode == 1 || mode == 3 ){
                    cyan();
                    printf("%d. ", line_num+1 );
                    reset();
                }
                if( mode == 2 || mode == 3 ){
                    cyan();
                    printf("%d. ", byte_offset );
                    reset();
                }
                my_print( f1.line[line_num], q, i, w, x, v );
            }
        }
        line_num++ ;
        byte_offset += len ;
    }
    if( mode == 4 ){
        if( h == 1 ){
            yellow();
            printf("%s : ", fname1 );
            reset();
        }
        cyan();
        printf("%d \n", no );
        reset();
    }
    return ;
}

void help(){
    printf("  -i -- ignore case distinctions in patterns and data \n");
    printf("  -w -- match only whole words \n");
    printf("  -x -- match only whole lines \n");
    printf("  -v -- select non-matching lines \n");
    printf("  -m -- stop after NUM selected lines \n");
    printf("  -b -- print the byte offset with output lines \n");
    printf("  -n -- print line number with output lines \n");
    printf("  -H -- print file name with output lines \n");
    printf("  -h -- suppress the file name prefix on output \n");
    printf("  -o -- show only nonempty parts of lines that match \n");
    printf("  -r -- to handle directories \n");
    printf("  -L -- print only names of FILEs with no selected lines \n");
    printf("  -l -- print only names of FILEs with selected lines \n");
    printf("  -c -- print only a count of selected lines per FILE \n");
    printf("  -E -- patterns are extended regular expressions \n");
    printf("  -q -- quit \n");
}

void version(){
    printf("grep (GNU grep) 14.5 \n");
    printf("DSA Project \n");
    printf("Project : Grep \n");
    printf("Written by Jayesh Gadilohar \n");
}