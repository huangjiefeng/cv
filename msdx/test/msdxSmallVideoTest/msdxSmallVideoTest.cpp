#include <conio.h>
#include <windows.h>
#include <process.h>
#include <assert.h>

#include <iostream>
using namespace std;

#include "msdx\config.h"
#include "exportutil.h"


int main(int argc, char* argv[])
{
	int video_width = 1920;
	int video_height = 1080;
	int video_port = 8600;
	char * video_ip = "127.0.0.1";
	int local_preview_port = 9100;
	char * local_preview_ip = "127.0.0.1";
	int rs;

    //init param
    if (argc >= 5)
    {
        video_width = atoi(argv[1]);
        video_height = atoi(argv[2]);
        video_port = atoi(argv[3]);
        local_preview_port = atoi(argv[4]);
    }

	rs = msdx_initial();
	assert(rs == 0);

	rs = msdx_create_smallvideo_graph("smallvideo");
	assert(rs == 0);

	rs = msdx_set_preview_video_codec(MSDX_CONF_VIDEO_CODEC_H264, video_width, video_height, 30, 16);
	assert(rs == 0);

	// no audio in small video graph
	rs = msdx_set_preview_address("0.0.0.0", 0, video_ip, video_port);
	assert(rs == 0);

	rs = msdx_connect_smallvideo_graph();
	assert(rs == 0);

	rs = msdx_add_smallvideo_address("0.0.0.0", 0, local_preview_ip, local_preview_port, true );
	assert(rs == 0);

	rs = msdx_run_smallvideo_graph();
	assert(rs == 0);

	printf("Graph is running, Press any key to stop...\n\n");
	getch();

	msdx_uninitial();

	return 0;
}
