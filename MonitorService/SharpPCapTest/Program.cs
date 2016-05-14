using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using SharpPcap.LibPcap;
using SharpPcap;
using System.Threading;
using PacketDotNet;

namespace SharpPCapTest
{
    class Program
    {
        private static ICaptureDevice dev;
        private static int total = 0;

        static void Main(string[] args)
        {
            dev = CaptureDeviceList.Instance[0];
            dev.Open(DeviceMode.Normal);

            dev.Filter = "tcp";
            dev.OnPacketArrival += new PacketArrivalEventHandler(dev_OnPacketArrival);

            
            dev.StartCapture();
            Thread.Sleep(Timeout.Infinite);

            /*Console.In.ReadLine();
            dev.StopCapture();
            Console.Out.WriteLine("total:" + total);
            Console.In.ReadLine();*/
        }

        static void dev_OnPacketArrival(object sender, CaptureEventArgs e)
        {
            Packet packet = IpPacket.ParsePacket(e.Packet.LinkLayerType, e.Packet.Data);

            IPv4Packet pv4 = packet.PayloadPacket as IPv4Packet;
            IPv6Packet pv6 = packet.PayloadPacket as IPv6Packet;

            if (pv4 != null)
            {
                total += pv4.TotalLength;
                String src = pv4.SourceAddress.ToString();
                String dst = pv4.DestinationAddress.ToString();

                Console.Out.WriteLine(src + " --> " + dst + "\tlen:" + pv4.TotalLength);
            }

            if (pv6 != null)
            {
                total += pv6.TotalLength;
                String src = pv6.SourceAddress.ToString();
                String dst = pv6.DestinationAddress.ToString();

                Console.Out.WriteLine(src + " --> " + dst + "\tlen:" + pv6.TotalLength);
            }

            //Console.Out.WriteLine(total);
        }

        private static void ReadPacket()
        {
            
        }
    }
}
