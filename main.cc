#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include "processpool.h"
#include "echo.h"

int main( int argc , char* argv[] )
{
    if (argc <= 2)
    {
        printf( "Usage: %s ip_address portname\n", argv[0] );
        return 0;
    }

    const char* ip = argv[1];
    int port = atoi( argv[2] );
                                                    
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 1 );

    struct sockaddr_in address;
    memset( &address, 0, sizeof( address ) );
    address.sin_family = AF_INET;
    address.sin_port = htons( port );
    inet_pton( AF_INET, ip, &address.sin_addr );

    int ret = 0;
    ret = bind( listenfd, (struct sockaddr*)( &address ), sizeof( address ) );
    assert( ret != -1 );

    ret = listen( listenfd, 5 );
    assert( ret != -1 );
    
    processpool<echo>* pool = processpool<echo>::create( listenfd, 8 );
    pool->run();

    close( listenfd );

    return 0;
}
