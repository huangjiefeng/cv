#pragma  once
/************************************************************************/
/* @brief 该测试用例用于尝试channeldispatcher的常规使用
*/
/************************************************************************/


#define VIDEOCAPTRUEDEVICE "\\\\?\\root#media#0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global"
//#define VIDEOCAPTRUEDEVICE "Logitech HD Webcam C270" //"USB2.0 UVC 1.3M WebCam"

//#define VIDEOCAPTRUEDEVICE "USB2.0 PC Camera" //"USB2.0 UVC 1.3M WebCam"
#define AUDIO_INPUT_DEVICE "Microphone #1 (e2eSoft VAudio)"
#define AUDIO_OUTPUT_DEVICE "Default DirectSound Device"

void ChannelTest();
void AudioTestMediaTest();
void VideoTestMediaTest();