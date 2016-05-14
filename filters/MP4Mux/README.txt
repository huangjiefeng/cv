1、此目录下的mpeg4.zip是官方原版代码压缩包。


* GDCL Mpeg4 Mux 修改

第一部分：改写为一个不依赖于 File Writer 的 Filter：
1、重要：改MuxOutput::Replace、MuxOutput::FillSpace方法，用CreateFile、SetFilePointerEx、WriteFile代替原写入语句。
2、去MuxOutput:CBaseOutputPin继承，改构造函数（其实无关紧要）。
3、去MuxOutput中的IStream等相关成员，去Mpeg4Mux中相关MuxOutput:CBaseOutputPin的调用。
4、Mpeg4Mux中GetPin、GetPinCount不再考虑输出Pin

第二部分：其他修正和变更：
1、把名字改成了SCUT Mpeg-4 Multiplexor Writer~
2、重要：修改SyncIndex::Write，只有存在关键帧索引才添加该Box
3、MovieWriter::CheckQueues()需要注释掉一些判断语句，使其不遵循严格的Interleaving，以支持音视频暂停发送时的处理

第三部分：增加文件分割功能：
原理：
a.OnReceiveSample()中发起分段，分段时，需要检测视频关键帧，从关键帧时间点开始分割
b.当接收到的某一Track的Sample时间戳大于分割点时，对该Track做EOS（End of stream，结束流），该Track之后的Sample放入新Movie对应的Track
c.当所有Track均已EOS，则关闭Movie
实现：
1、MuxInput所有对m_pTrack的调用改为通过从Mpeg4Mux::GetTrack获取TrackWriter指针
2、修改Mpeg4Mux::MakeTrack的行为，创造的TrackWriter在Mux内部管理，用Map记录其对应的MediaType，以便下次创建Track
3、修改Mpeg4Mux::Pause、Mpeg4Mux::Stop、和MuxInput::Receive，以调用新的方法
4、增加实现了ISCUTMpeg4Mux接口
5、MovieWriter::Close中*pDuration = tDur;改为*pDuration += tDur;，即将分段的时间累积

第四部分：实时流音视频同步
1、在OnReceiveSample中，录制首个分段时，检测第一个视频关键帧到达的时间，并在此时间之后的音频帧才被录制

第五部分：针对发送端暂停音视频发送的处理
ISO方法：根据ISO-IEC-14496-12、ISO_IEC_14496-14标准，可在ELST BOX中定义间断信息。
1、修改Duration的Add、OffsetTime、WriteEdts三个方法，详见代码，并参考Edit list box相关资料。
2、说明：经测试，大多数播放器会忽略ELST信息，仅Osmo4播放器支持视频的ELST，VLC播放器仅能较好的处理开头的empty edit，其他播放器完全忽略。故通过m_bUseElst变量，关闭了这个方法。2013-5

第六部分：不要写入空Track
空的Track会造成部分播放器无法播放文件。
1、在TrackWriter中增加m_lLength记录写入字节数。
2、MovieWriter::Close方法中，增加空轨检测机制，并忽略写入空轨。注意：有一个部位写入轨道数目，要减去空轨数目。


第八部分：增加文件写入缓冲区
详见FileWriter

其他修改：
1、增加文件名格式化命名（与上层耦合），详见MuxFilter.cpp中PrintFileName函数。
2、增加录制状态报告。以RecordStat结构返回数据。
3、H264ByteStreamHandler::WriteDescriptor中增加对H.264码流检测其真实的图像大小。



