using System;
using System.Collections;

using System.Collections.Specialized;

namespace RemoteMonitor
{
    class ProcessInfo
    {
        public int PID { set; get; }
        public String Name { set; get; }
        public String Priority { set; get; }
        public String MemoryUse { set; get; }
        public double CpuTime { set; get; }
        public int ThreadCount { set; get; }
        public double CpuUsage { set; get; }
        public bool Mark { set; get; }

        public ProcessInfo()
        {
            Mark = false;
        }

        public ProcessInfo(String info)
        {
            String[] infos = info.Split(',');
            PID = Convert.ToInt32(infos[0]);
            Name = infos[1];
            Priority = infos[2];
            MemoryUse = infos[3];
            try
            {
                CpuTime = Convert.ToDouble(infos[4]);
            }
            catch { }
            try
            {
                ThreadCount = Convert.ToInt32(infos[5]);
            }
            catch { }
            Mark = false;
        }

        public void CopyFrom(ProcessInfo other)
        {
            PID = other.PID;
            Name = other.Name;
            Priority = other.Priority;
            MemoryUse = other.MemoryUse;
            CpuTime = other.CpuTime;
            ThreadCount = other.ThreadCount;
            CpuUsage = other.CpuUsage;
        }
    }

    class ProcessInfoCollection : CollectionBase, INotifyCollectionChanged
    {
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void Add(ProcessInfo info)
        {
            this.List.Add(info);
            if (CollectionChanged != null)
            {
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, info));
            }
        }
        
        public new void Clear()
        {
            base.Clear();
            if (CollectionChanged != null)
            {
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
            }
        }

        public void Remove(ProcessInfo process)
        {
            this.List.Remove(process);
            if (CollectionChanged != null)
            {
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, process));
            }
        }
    }

    class ThreadInfo
    {
        public int TID { set; get; }
        public String Priority { set; get; }
        public String State { set; get; }
        public double CpuTime { set; get; }
        public double CpuUsage { set; get; }

        public ThreadInfo()
        {
        }

        public ThreadInfo(String info)
        {
            String[] infos = info.Split(',');
            TID = Convert.ToInt32(infos[0]);
            Priority = infos[1];
            State = infos[2];
            try
            {
                CpuTime = Convert.ToDouble(infos[3]);
            }
            catch
            {
            }
        }
    }

    class ThreadInfoCollection : CollectionBase, INotifyCollectionChanged
    {
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void Add(ThreadInfo info)
        {
            this.List.Add(info);
            if (CollectionChanged != null)
            {
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, info));
            }
        }

        public new void Clear()
        {
            base.Clear();
            if (CollectionChanged != null)
            {
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
            }
        }
    }
}