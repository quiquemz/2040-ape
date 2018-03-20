#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "thinkgear.h"

#define WINVER 0x0500
#include "Windows.h"

/**
 * Prompts and waits for the user to press ENTER.
 */
void
wait() {
    printf( "\n" );
    printf( "Press the ENTER key...\n" );
    fflush( stdout );
    getc( stdin );
}
bool KeyPressed(int a){
	if (GetAsyncKeyState(a)& 0x8000)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void moveLeft(INPUT ip) {
	// Press the "left" key
	ip.ki.wVk = 0x25; // virtual-key code for the "a" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));
	// Release the "left" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}
void moveUp(INPUT ip) {
	// Press the "up" key
	ip.ki.wVk = 0x26; // virtual-key code for the "a" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));
	// Release the "up" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}
void moveRight(INPUT ip) {
	// Press the "right" key
	ip.ki.wVk = 0x27; // virtual-key code for the "a" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));
	// Release the "right" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}
void moveDown(INPUT ip) {
	// Press the "down" key
	ip.ki.wVk = 0x28; // virtual-key code for the "a" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));
	// Release the "down" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}

/**
 * Program which prints ThinkGear Raw Wave Values to stdout.
 */
int
main( void ) {
    
	//switching modes
	int UP_DOWN = 0;
	int RIGHT_LEFT = 1;
	int mode = UP_DOWN;
	
	//code for keyboard input
	INPUT ip;
	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	// left 0x25, up 0x26, right 0x27, down 0x28

    char *comPortName  = NULL;
    int   dllVersion   = 0;
    int   connectionId = 0;
    int   packetsRead  = 0;
    int   errCode      = 0;
    
    double secondsToRun = 0;
    time_t startTime    = 0;
    time_t currTime     = 0;
    char  *currTimeStr  = NULL;
	int set_filter_flag = 0;
	int count = 0;
    
    /* Print driver version number */
    dllVersion = TG_GetVersion();
    printf( "Stream SDK for PC version: %d\n", dllVersion );
    
    /* Get a connection ID handle to ThinkGear */
    connectionId = TG_GetNewConnectionId();
    if( connectionId < 0 ) {
        fprintf( stderr, "ERROR: TG_GetNewConnectionId() returned %d.\n",
                connectionId );
        wait();
        exit( EXIT_FAILURE );
    }
    
    /* Set/open stream (raw bytes) log file for connection */
    errCode = TG_SetStreamLog( connectionId, "streamLog.txt" );
    if( errCode < 0 ) {
        fprintf( stderr, "ERROR: TG_SetStreamLog() returned %d.\n", errCode );
        wait();
        exit( EXIT_FAILURE );
    }
    
    /* Set/open data (ThinkGear values) log file for connection */
    errCode = TG_SetDataLog( connectionId, "dataLog.txt" );
    if( errCode < 0 ) {
        fprintf( stderr, "ERROR: TG_SetDataLog() returned %d.\n", errCode );
        wait();
        exit( EXIT_FAILURE );
    }
    
    /* Attempt to connect the connection ID handle to serial port "COM5" */
    /* NOTE: On Windows, COM10 and higher must be preceded by \\.\, as in
     *       "\\\\.\\COM12" (must escape backslashes in strings).  COM9
     *       and lower do not require the \\.\, but are allowed to include
     *       them.  On Mac OS X, COM ports are named like
     *       "/dev/tty.MindSet-DevB-1".
     */
    comPortName = "\\\\.\\COM4";
    errCode = TG_Connect( connectionId,
                         comPortName,
                         TG_BAUD_57600,
                         TG_STREAM_PACKETS );
    if( errCode < 0 ) {
        fprintf( stderr, "ERROR: TG_Connect() returned %d.\n", errCode );
        wait();
        exit( EXIT_FAILURE );
    }
    
    /* Keep reading ThinkGear Packets from the connection for 5 seconds... */
    secondsToRun = 5;
    startTime = time( NULL );
    while( difftime(time(NULL), startTime) < secondsToRun ) {
        
        /* Read all currently available Packets, one at a time... */
        do {
            
            /* Read a single Packet from the connection */
            packetsRead = TG_ReadPackets( connectionId, 1 );
            
            /* If TG_ReadPackets() was able to read a Packet of data... */
            if( packetsRead == 1 ) {
                
                /* If the Packet containted a new raw wave value... */
                if( TG_GetValueStatus(connectionId, TG_DATA_MEDITATION) != 0 ) {
                    
                    /* Get the current time as a string */
                    currTime = time( NULL );
        			currTimeStr = ctime( &currTime );
                    
                    /* Get and print out the new raw value */
                    fprintf( stdout, "%s: raw: %d\n", currTimeStr,
                            (int)TG_GetValue(connectionId, TG_DATA_MEDITATION) );
                    fflush( stdout );
                    
                } /* end "If Packet contained a raw wave value..." */
                
            } /* end "If TG_ReadPackets() was able to read a Packet..." */
            
        } while( packetsRead > 0 ); /* Keep looping until all Packets read */
        
    } /* end "Keep reading ThinkGear Packets for 5 seconds..." */

	printf("auto read test begin:\n");
	fflush(stdin);
	errCode = TG_EnableAutoRead(connectionId,1);
	if(errCode == 0){
		packetsRead =0;
        errCode = MWM15_setFilterType(connectionId,MWM15_FILTER_TYPE_50HZ);//MWM15_FILTER_TYPE_60HZ
		printf("MWM15_setFilterType: %d\n",errCode);
		while(packetsRead <2000){
			//pressed space bar
			/*if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				printf("SPACE");
			}*/
			if ((1 << 15) & GetAsyncKeyState(VK_SPACE))
			{
				printf("SPACE");
			}
			/*if (KeyPressed(VK_SPACE)) {
				printf("SPACE");
			}
			If raw value has been updated ... */
            if( TG_GetValueStatus(connectionId, TG_DATA_MEDITATION) != 0 ) {
				int input = (int)TG_GetValue(connectionId, TG_DATA_MEDITATION);
                /* Get and print out the updated raw value */
                printf( "%d ", input);
				if (input >= 80) {
					moveUp(ip);
				}
				fflush( stdout );
				/*if(packetsRead % 10 == 0){ // too much stdout operation will lose some data
					int input = (int)TG_GetValue(connectionId, TG_DATA_MEDITATION);
				printf(  " %d ", input );
				}else{
					TG_GetValue(connectionId, TG_DATA_MEDITATION);
				}*/
				packetsRead ++;

	            //wait for a while to call MWM15_getFilterType
				if(packetsRead == 800 || packetsRead == 1600  ){// at lease 1s after MWM15_setFilterType cmd
					set_filter_flag ++;
					errCode = MWM15_getFilterType(connectionId);

					printf(  " \nMWM15_getFilterType   result: %d  index: %d\n",errCode,packetsRead );

				}

            }

			if(TG_GetValueStatus(connectionId, MWM15_DATA_FILTER_TYPE) != 0 ) {

                /* Get and print out the updated raw value */
				printf(  "\n#### @@@ Find Filter type: %d  set_filter_flag: %d  index: %d\n",
                         (int)TG_GetValue(connectionId, MWM15_DATA_FILTER_TYPE), set_filter_flag,packetsRead);
				break;
                
            } 
             
		}

		errCode = TG_EnableAutoRead(connectionId,0); //stop
		printf("auto read test stoped: %d \n",errCode);
	}else{
		printf("auto read test failed: %d \n",errCode);
	}

	TG_Disconnect(connectionId); // disconnect test
    
    /* Clean up */
    TG_FreeConnection( connectionId );
    
    /* End program */
    wait();
    return( EXIT_SUCCESS );
}

