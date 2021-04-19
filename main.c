#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include "grep.h"
#include "basic.h"
#include "color.h"
#include "rexp.h"

void execute_grep( int argc, char **argv );

int main(int argc, char *argv[]){
    if( argc == 1 ){
        printf("Usage: grep [OPTION]... PATTERNS [FILE]... \n");
        printf("Try 'grep --help' for more information. \n");
    }
    else if( argc == 2 && strcmp( argv[1], "--help" ) )
        help();
    else if( argc == 2 && strcmp( argv[1], "--version" ) )
        version();
    else if( argc >=3 )
	    execute_grep( argc, argv );
    else{
        printf("Usage: grep [OPTION]... PATTERNS [FILE]... \n");
        printf("Try 'grep --help' for more information. \n");
    }
    return 0 ;
}

void execute_grep( int argc, char **argv ){
    if( argc < 3 ){
        return ;
    }

    int b = 0, c = 0, f = 0, h = -1, i = 0, l = 0, n = 0, m = INT_MAX, o = 0, r = 0, v = 0, w = 0, x = 0, E = 0 ;
    int j, k, m_count = 0, s = 0, len ;
    int y = 0, z, mode ;
    char *q ;

    char *p ;
    j = 1 ;

    while( j < argc && argv[j][0] == '-' ){
        p = argv[j] ;
        p++ ;
        while( *p != '\0' ){
            switch ( *p )
            {
                case 'b' :
                    b = 1 ; break ; 
                case 'c' :
                    c = 1 ; break ;
                case 'E' :
                    E = 1 ; break ;
                case 'f' :
                    f = 1 ; break ;
                case 'h' :
                    h = 0 ; break ;
                case 'H' :
                    h = 1 ; break ;
                case 'i' :
                    i = 1 ; break ;
                case 'l' :
                    l = 1 ; break ;
                case 'L' :
                    l = 2 ; break ;
                case 'm' :
                    m = 0 ;
                    if( p[1] == '\0' && (j+1) < argc )
                        p = argv[++j] ;
                    else if( p[1] >= '0' && p[1] <= '9' )
                        p++ ;
                    else{
                        printf("grep: Invalid max count..!! \n");
                        return ;
                    }
                    while( *p != '\0' ){
                        if( *p >= '0' && *p <= '9' )
                            m = m*10 + ( *p - '0' );
                        else{
                            printf("grep: Invalid max count..!! \n");
                            return ;
                        }
                        p++ ;
                    }
                    p-- ;
                    break;
                case 'n' :
                    n = 1 ; break ;
                case 'o' :
                    o = 1 ; break ;
                case 'r' :
                    r = 1 ; break ;
                case 'v' :
                    v = 1 ; break ;
                case 'w' :
                    w = 1 ; break ;
                case 'x' :
                    x = 1 ; break ;
                case 'q' :
                    printf("Quiting..!! \n");
                    return ;
                    break ;
                default:
                    printf("grep: invalid option -- 'j' \n");
                    printf("Usage: grep [OPTION]... PATTERNS [FILE]... \n");
                    printf("Try 'grep --help' for more information. \n");
                    return ;
                    break ;
            }
            p++ ;
        }
        j++ ;
    }
    if( ( argc -j ) < 2 )
        return ;
    if( m_count > 1 )
        return ;

    if( o && v ){
        if( c || l )
            o = 0 ;
        else
            return;
    }

    if( c )
        mode = 4 ;
    else
        mode = n + b*2 ;

    if( l && v ){
        if( l == 1 )
            l = 2 ;
        else
            l = 1 ;
    }

    len = strlen(argv[j]);
    if( argv[j][0] == '^' || argv[j][len-1] == '$' ){
        if( argv[j][0] == '^' && argv[j][len-1] == '$' ){
            y = 1 ;
            len-- ;
            x = 1 ;
        }
        else if( argv[j][0] == '^' ){
                y = 1 ;
                s = 1 ;
        }
        else if( argv[j][len-1] == '$' )
            s = 2 ;

        q = (char*)malloc(len-1) ;
        len-- ;
        for( z = 0 ; z < len ; z++ )
            q[z] = argv[j][y++] ;
    }
    else
        q = argv[j] ;

    j++ ;
    if( o && x )
        o = 0 ;
    
    if( r ){
        if( h == -1 )
            h = 1 ;
        if( ( argc -j ) == 1  && isfile(argv[j]) == 2 )
            grep_directory( argv[j], q, mode, i, v, w, x, h, m, l, o, s, E );
        return ;
    }

    else if( f ){
        if( ( argc -j ) == 1 && isfile(q) == 2 && isfile(argv[j]) == 2 )
            grep_f( q, argv[j], mode, i, v, w, x, h, m );
        return ;
    }

    if( h == -1 && (argc -j) > 1 )
            h = 1;
    if( l ){
        for( k = j ; k < argc ; k++ )
            if( isfile(argv[k]) == 1 )
                grep_l( argv[k], q, l, i, w, x, s, E );
    }
    else if( o ){
        for( k = j ; k < argc ; k++ )
            if( isfile(argv[k]) == 1 )
                grep_o( argv[k], q, mode, i, w, h, m, s, E );
    }
    else{
        for( k = j ; k < argc ; k++ )
            if( isfile(argv[k]) == 1 )
                grep( argv[k], q, mode, i, v, w, x, h, m, s, E );
    }
    return ;
}