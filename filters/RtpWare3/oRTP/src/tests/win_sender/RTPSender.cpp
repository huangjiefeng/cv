#include <ortp/ortp.h>
#include <string.h>
#include <tchar.h>

#ifndef WIN32
#define  WIN32
#endif


#define STREAMS_COUNT 1000

enum
{
    EVENT_STOP,
    EVENT_RTP,
    EVENT_COUNT						//  Always last
};


RtpSession *	m_Session[STREAMS_COUNT];

int				m_nPacket_Size		= 160;
int				m_nTimestamp_Inc	= 100;

char		*	m_pBuffer			= NULL;
char		*	m_SSRC				= NULL;

int				m_nChannels			= 0;
int				m_nPort				= 0;

HANDLE			m_hEvents[EVENT_COUNT];

BOOL			m_bExit				= FALSE;

static char *help = "usage: mrtpsend	filename ip port nstreams [--packet-size size] [--ts-inc value]\n";

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
			ortp_fatal("Bad level !");
	}

	_tprintf_s(lname);
	_vtprintf_s(fmt, args);
	_tprintf_s("\n");
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
        SetEvent(m_hEvents[EVENT_STOP]);
        return TRUE;

    default:
        return FALSE;
    }
}

int GetCommandArguments(int argc, char *argv[])
{
    int				nCounter;


    // Check the number of arguments
    if (argc < 5)
    {
        printf(help);
        return -1;
    }

    m_nChannels = atoi(argv[4]);

    // Get the number of channels
    if (m_nChannels == 0)
    {
        printf(help);
        return -1;
    }

    /* look at command line options */
    for (nCounter = 5; nCounter < argc; nCounter++)
    {
        if (strcmp(argv[nCounter], "--packet-size") == 0)
        {
            if ( nCounter + 1 < argc )
            {
                m_nPacket_Size = atoi(argv[nCounter+1]);
            }
            else
            {
                printf(help);
                return -1;
            }
            if (m_nPacket_Size == 0)
            {
                printf("Packet size can't be %s.\n", argv[nCounter+1]);
                return -1;
            }
            nCounter += 1;

        }
        else if (strcmp(argv[nCounter], "--ts-inc") == 0)
        {
            if ( nCounter + 1 < argc )
            {
                m_nTimestamp_Inc = atoi(argv[nCounter+1]);
            }
            else
            {
                printf(help);
                return -1;
            }
            if (m_nTimestamp_Inc == 0)
            {
                printf("Timestanp increment can't be %s.\n", argv[nCounter+1]);
                return -1;
            }

            nCounter += 1;
        }
    }

    return 0;
}

void ProductVersion()
{
    char	strBuffer[255];

    printf("====================================\n");
    printf("Author  : Simon Morlat             =\n");
    printf("Porting : Yann STEPHAN             =\n");
    printf("====================================\n");

    memset(&strBuffer, 0x0, sizeof(strBuffer));

    sprintf((char *) &strBuffer, "= RTPSender V1.0   - Date : %s - %s\n", __DATE__, __TIME__);
    printf(strBuffer);

    printf("====================================\n");
}

int __cdecl main(int argc, char *argv[])
{
    //argc =5;
    //argv[1] ="d:\sound.wav";
    //argv[2] ="202.38.250.236";
    //argv[3] ="9999";
    //argv[4] ="1";


    FILE		*	infile				= NULL;
    SessionSet	*	pSessionSet			= NULL;
    int				nCounter			= 0;
    UINT32			m_nUser_Timestamp	= 0;


	ortp_set_log_file(stdout);
	ortp_set_log_level_mask(ORTP_MESSAGE|ORTP_WARNING|ORTP_ERROR|ORTP_FATAL|ORTP_DEBUG);
	//	ortp_set_log_file(logFile);
	ortp_set_log_handler(My_ortp_logv_out);

    ortp_message("hello ortp\r\n");
    ortp_debug("ortp_debug\r\n");


    ProductVersion();

    if (GetCommandArguments(argc, argv) != 0)
    {
        printf("==> Sorry dude...\n");
        Sleep(1000);
        return -1;
    }

    printf("==> Starting the RTP Sender test\n");

    // =============== INSTALL THE CONTROL HANDLER ===============
    if (SetConsoleCtrlHandler( (PHANDLER_ROUTINE) ctrlHandlerFunction, TRUE) == 0)
    {
        printf("==> Cannot handle the CTRL-C...\n");
    }


    printf("==> Timestamp increment will be %i\n"	, m_nTimestamp_Inc);
    printf("==> Packet size will be %i\n"			, m_nPacket_Size);

    m_pBuffer = (char *) ortp_malloc(m_nPacket_Size);

    ortp_init();
    ortp_scheduler_init();
    printf("==> Scheduler initialized\n");

    m_SSRC	= getenv("SSRC");
    m_nPort	= atoi(argv[3]);

    //  int ssrc = atoi(m_SSRC);
    for (nCounter = 0; nCounter < m_nChannels; nCounter++)
    {
        printf("==> Channel [#%d]\n", nCounter);

        m_Session[nCounter] = rtp_session_new(RTP_SESSION_SENDONLY);

        rtp_session_set_scheduling_mode(m_Session[nCounter], 1);
        rtp_session_set_blocking_mode(m_Session[nCounter], 0);
        rtp_session_set_remote_addr(m_Session[nCounter], argv[2], m_nPort);
	//	rtp_session_set_local_addr(m_Session[nCounter], /*argv[2]*/"224.1.2.5", m_nPort);
        rtp_session_set_send_payload_type(m_Session[nCounter], 0);
        rtp_session_set_dscp(m_Session[nCounter], 40);
		rtp_session_set_multicast_loopback(m_Session[nCounter],true);
        if (m_SSRC != NULL)
        {
            rtp_session_set_ssrc(m_Session[nCounter], atoi(m_SSRC));
        }
		rtp_session_enable_rtcp(m_Session[nCounter],true);

        m_nPort += 2;
    }

    infile = fopen(argv[1], "r");

    if (infile == NULL)
    {
        printf("==> Cannot open file !!!!");
        Sleep(1000);
        return -1;
    }

    printf("==> Open file\n");

    /* Create a set */
    pSessionSet = session_set_new();
//	printf("==> Session set\n");

    nCounter = m_nPacket_Size;
    m_pBuffer = new char[m_nPacket_Size];
    memset(m_pBuffer, 1, m_nPacket_Size * sizeof(char));
    int seq = 0;
    int index = 0;
	SYSTEMTIME st;
    //while( ((nCounter = (int) fread(m_pBuffer, 1, m_nPacket_Size, infile)) > 0) && (m_bExit == FALSE) )
    while( (m_bExit == FALSE) )
    {
       // for(int round = 0; round < 10; round++)
        {
            int k;
            //printf("Sending packet.");
            for (k = 0; k < m_nChannels; k++)
            {
                /* add the session to the set */
                session_set_set(pSessionSet, m_Session[k]);
                //printf("==> Session set set %d\n", k);
            }
            /* and then suspend the process by selecting() */
            session_set_select(NULL, pSessionSet, NULL);
            //printf("==> Session set select\n");

            for (k = 0; k < m_nChannels; k++)
            {
                //printf("---\n");
                /* this is stupid to do this test, because all session work the same way,
                as the same user_ts is used for all sessions, here. */

                if (session_set_is_set(pSessionSet, m_Session[k]))
                {
                    //乱序发送
                    //	mblk_t* mp = rtp_session_create_packet(m_Session[k], RTP_FIXED_HEADER_SIZE, (uint8_t*)m_pBuffer, nCounter);
                    //	rtp_set_seqnumber(mp, seq);
                    //	rtp_set_timestamp(mp, m_nUser_Timestamp);
                    //rtp_set_ssrc(mp,ssrc);
                    //	rtp_set_payload_type(mp, 0);
                    // printf("==> Session set is set %d\n", k);
				//	GetSystemTime(&st);
				//m_nUser_Timestamp = st.wMilliseconds; //获取系统时间
                    rtp_session_send_with_ts(m_Session[k], (uint8_t *)m_pBuffer, nCounter, m_nUser_Timestamp);
                    //    rtp_session_sendm_with_ts(m_Session[k], mp, m_nUser_Timestamp);
                 printf("sending packet: seq\t%d,timestamp\t%d \n", seq, m_nUser_Timestamp);
                }
            }
            //rtp_session_send_with_ts(m_Session[0],(uint8_t *)m_pBuffer,nCounter,m_nUser_Timestamp);

            //if (index % 2 == 0)
            //{
            //    seq += 2;
            //    m_nUser_Timestamp += m_nTimestamp_Inc * 2;
            //}
            //else
            //{
            //    seq -= 1;
            //    m_nUser_Timestamp -= m_nTimestamp_Inc;
            //}
            seq++;
            index++;
        }
	//	Sleep(10);
       m_nUser_Timestamp += m_nTimestamp_Inc;
    }

    fclose(infile);
    printf("==> Close file\n");

    for(nCounter = 0; nCounter < m_nChannels; nCounter++)
    {
        rtp_session_destroy(m_Session[nCounter]);
    }

    session_set_destroy(pSessionSet);

    // Give us some time
    Sleep(50);

    ortp_exit();
   ortp_global_stats_display();
	// printStat();

    ortp_free(m_pBuffer);

    printf("==> Remove the CTRL-C handler...\n");
    SetConsoleCtrlHandler( (PHANDLER_ROUTINE) ctrlHandlerFunction, FALSE);

    // Wait for an input key
    printf("Waiting for exit : ");

    for (nCounter = 0; nCounter < 4 * 5; nCounter++)
    {
        printf(".");
        Sleep(50);
    }

    return 0;
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