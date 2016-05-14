//#define  ORTP_INET6
#include <ortp/ortp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <tchar.h>

#define STREAMS_COUNT 1000

const int				m_nTimestamp_Inc	= 160;

BOOL			m_bExit			= FALSE;

FILE * logFile = NULL;

static char *help = "usage: mrtprecv	file_prefix local_port number_of_streams \n"
                    "Receives multiples rtp streams on local_port+2*k, k={0..number_of_streams}\n";

void printStat();

static void My_ortp_logv_out(OrtpLogLevel lev, const char *fmt, va_list args){
	char *lname="";
	switch(lev){
		case ORTP_DEBUG:
			lname="debug: ";
			break;
		case ORTP_MESSAGE:
			lname="message: ";
			break;
		case ORTP_WARNING:
			lname="warning: ";
			break;
		case ORTP_ERROR:
			lname="error: ";
			break;
		case ORTP_FATAL:
			lname="fatal: ";
			break;
		default:
			lname="Bad level !";
	}

	_tprintf_s(lname);
	_vtprintf_s(fmt, args);
	_tprintf_s("\n");
}



void log(const char * format, ...);

void ProductVersion()
{
    char	strBuffer[255];

    printf("====================================\n");
    printf("Author  : Simon Morlat             =\n");
    printf("Porting : Yann STEPHAN             =\n");
    printf("====================================\n");

    memset(&strBuffer, 0x0, sizeof(strBuffer));

    sprintf((char *) &strBuffer, "= RTPReceiver V1.0   - Date : %s - %s\n", __DATE__, __TIME__);
    printf(strBuffer);

    printf("====================================\n");
}

BOOL ctrlHandlerFunction(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
        // Handle the CTRL+C signal.
        // CTRL+CLOSE: confirm that the user wants to exit.
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        m_bExit = TRUE;
        return TRUE;

    default:
        return FALSE;
    }
}

int rtp2disk(RtpSession *session, uint32_t& ts, FILE * fd)
{
    //char buffer[160];
    //int err,havemore=1;



    unsigned char *buffer;
    mblk_t *mp;

	SYSTEMTIME st;
    while (true)
    {
        mp = rtp_session_recvm_with_ts(session, ts);
        if (mp == NULL)
            return 0;
        int seq = rtp_get_seqnumber(mp);
       // int size = rtp_get_payload(mp, &buffer);
        int timestamp = rtp_get_timestamp(mp);

		GetSystemTime(&st);
        printf("seq %d\t ts %d\tuts  %d\r\n", seq, timestamp,ts);
		
		//freemsg(mp);		
        /*if (size > 0)
        {
          	fwrite(&buffer, 1, size, fd);
        }*/
    }

    //while (havemore)
    //{
    //
    //	err=rtp_session_recv_with_ts(session,(uint8_t *)buffer,160,ts,&havemore);
    //
    //	//mblk_t *mp= 	rtp_session_recvm_with_ts(session,ts);
    //
    //	if (havemore)
    //		printf("==> Warning: havemore=1!\n");

    //	if (err>0)
    //	{
    //		rtp_session_set_data(session,(void*)1);
    //		/* to indicate that (for the application) the stream has started, so we can start
    //		recording on disk */
    //		printf("==> receive timestamp : %d\n",ts);
    //	}

    //	if  (session->user_data != NULL)
    //	{
    //		fwrite(&buffer,1,160, fd);
    //	}
    //}
    return 0;
}

int GetSystemInformation()
{
    SYSTEM_INFO	SystemInfo;

    GetSystemInfo(&SystemInfo);

    return SystemInfo.dwNumberOfProcessors;
}

int __cdecl main(int argc, char *argv[])
{

#ifdef ORTP_INET6
	cout<<"ORTP_INET6 defined;"<<endl;
#endif

    logFile = fopen("log.txt", "w");

	//ortp_set_log_file(NULL);
	ortp_set_log_file(stdout);
	ortp_set_log_level_mask(ORTP_MESSAGE|ORTP_WARNING|ORTP_ERROR|ORTP_FATAL|ORTP_DEBUG);
	//	ortp_set_log_file(logFile);
	ortp_set_log_handler(My_ortp_logv_out);
	//	ortp_set_log_level_mask(0);
	//ortp_set_log_level_mask(ORTP_MESSAGE|ORTP_WARNING|ORTP_ERROR);
//
	// ortp_log_level_enabled(TRUE);

    RtpSession	*	session[STREAMS_COUNT];
    FILE		*	filefd[STREAMS_COUNT];
    SessionSet	*	set;

    uint32_t			user_ts				= 0;

    int			port				= 0;
    int			channels			= 0;
    int			i					= 0;
    int			nCPUCount			= 0;
    int			nSchedulerCPU		= 2;

    char			strFilename[MAX_PATH];

    ProductVersion();

    if (argc < 5)
    {
        printf(help);
        //	return -1;


    }

    channels = atoi(argv[4]);
    if (channels == 0)
    {
        printf(help);
        return -1;
    }

    // Now it's time to use the power of multiple CPUs
    nCPUCount = GetSystemInformation();

    printf("==> # of CPU detected : %d\n", nCPUCount);

    ortp_init();
    ortp_scheduler_init();

    if (nCPUCount > 1)
    {
        if (nCPUCount > 2)
        {
            nSchedulerCPU	= 3;
        }

        /*
        		if (ortp_bind_scheduler_to_cpu(nSchedulerCPU) != -1)
        			{
        				printf("==> Scheduler has been binded to CPU %d\n", nSchedulerCPU);
        			}
        			else
        			{
        				printf("==> Scheduler still binded to CPU 1\n");
        			}
        			*/

    }

    port = atoi(argv[3]);

	printf("port: %d\n",port);
    for (i = 0; i < channels; i++)
    {
        session[i] = rtp_session_new(RTP_SESSION_RECVONLY);
        rtp_session_set_scheduling_mode(session[i], 1);
        rtp_session_set_blocking_mode(session[i],0);
		rtp_session_set_recv_payload_type(session[i],0);
		rtp_session_set_local_addr(session[i],argv[2], port);
	//	rtp_session_set_local_addr(session[i],"224.1.2.5", port);
	//	rtp_session_set_remote_addr(session[i],argv[2], port);
       // rtp_session_set_send_payload_type(session[i], 0);
       rtp_session_enable_adaptive_jitter_compensation(session[i], TRUE);
    //  rtp_session_set_recv_buf_size(session[i], 1048);
		//rtp_session_set_send_payload_type(m_session,0);
		//  rtp_session_enable_adaptive_jitter_compensation(m_session, TRUE);
	// rtp_session_enable_jitter_buffer(session[i], FALSE);
	//	rtp_session_set_jitter_compensation(session[i], 0);      
	rtp_session_enable_rtcp(session[i],true);
		port += 2;
    }


    memset(strFilename, 0x0, sizeof(strFilename));

    for (i = 0; i < channels; i++)
    {
        sprintf(strFilename, "%s%4.4d.dat", argv[1], i);

        filefd[i] = fopen(strFilename, "wb");

        if (filefd[i] < 0)
        {
            printf("Could not open %s for writing: %s", strFilename, strerror(errno));
        }
    }

    // =============== INSTALL THE CONTROL HANDLER ===============
    if (SetConsoleCtrlHandler( (PHANDLER_ROUTINE) ctrlHandlerFunction, TRUE) == 0)
    {
        printf("==> Cannot handle the CTRL-C...\n");
    }

    /* create a set */
    set = session_set_new();
    printf("==> RTP Receiver started\n");
	
	ortp_global_stats_reset();

    while(m_bExit == FALSE)
    {
        int k;

        for (k = 0; k < channels; k++)
        {
            /* add the session to the set */
            session_set_set(set, session[k]);
            //printf("session_set_set %d\n", k);
        }
        /* and then suspend the process by selecting() */
        k = session_set_select(set, NULL, NULL);
        //printf("session_set_select\n");
        if (k == 0)
        {
            printf("==> Warning: session_set_select() is returning 0...\n");
        }

        for (k = 0; k < channels; k++)
        {
          if (session_set_is_set(set, session[k]))
            {
              //  printf("==>uts %d\n", user_ts);
                //接收处理包
            rtp2disk(session[k], user_ts, filefd[k]);
            }          
        }
        user_ts += m_nTimestamp_Inc;
    }

    printf("==> Exiting\n");

    for (i = 0; i < channels; i++)
    {
        fclose(filefd[i]);
        rtp_session_destroy(session[i]);
    }
    session_set_destroy(set);

    ortp_exit();

   ortp_global_stats_display();
	 //printStat();

    printf("Waiting for exit : ");

    for (i = 0; i < 4 * 5; i++)
    {
        printf(".");
        Sleep(50);
    }

    fclose(logFile);

    return 0;
}


void log(const char * format, ...)
{
    va_list vaList;
    va_start(vaList, format);
    printf( format, vaList);
    fprintf(logFile, format, vaList);
    va_end(vaList);
}

void printStat()
{
	rtp_stats_t * stat =ortp_get_global_stats();
	printf(		"oRTP-stat:\n :");
	printf(
		" number of rtp packet sent=%I64d\n",
		(uint64_t)stat->packet_sent);
	printf(
		" number of rtp bytes sent=%I64d bytes\n",
		(uint64_t)stat->sent);
	printf(
		" number of rtp packet received=%I64d\n",
		(uint64_t)stat->packet_recv);
	printf(
		" number of rtp bytes received=%I64d bytes\n",
		(uint64_t)stat->hw_recv);
	printf(
		" number of incoming rtp bytes successfully delivered to the application=%I64d \n",
		(uint64_t)stat->recv);
	printf(
		" number of times the application queried a packet that didn't exist=%I64d \n",
		(uint64_t)stat->unavaillable);
	printf(
		" number of rtp packet lost=%I64d\n",
		(uint64_t) stat->cum_packet_loss);
	printf(
		" number of rtp packets received too late=%I64d\n",
		(uint64_t)stat->outoftime);
	printf(
		" number of bad formatted rtp packets=%I64d\n",
		(uint64_t)stat->bad);
	printf(
		" number of packet discarded because of queue overflow=%I64d\n",
		(uint64_t)stat->discarded);
}