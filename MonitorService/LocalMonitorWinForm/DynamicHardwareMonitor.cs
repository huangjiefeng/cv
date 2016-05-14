using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Collections.ObjectModel;
using OpenHardwareMonitor.GUI;
using OpenHardwareMonitor.Hardware;
using OpenHardwareMonitor.Utilities;
using System.Diagnostics;

namespace LocalMonitorWinForm
{
    public class DynamicHardwareMonitor : IHardwareMonitor
    {
        public const string GPU_ENABLE_KEY = "GPUEnable";
        public const string CPU_ENABLE_KEY = "CPUEnable";
        public const string RAM_ENABLE_KEY = "RAMEnable";
        public const string HDD_ENABLE_KEY = "HDDEnable";
        public const string MAINBOARD_ENABLE_KEY = "MainboardEnable";
        public const string FAN_CONTROLLER_ENABLE_KEY = "FanControllerEnable";

        private PersistentSettings settings;
        private UnitManager unitManager;
        private Computer computer;
        private Node root;
        private IVisitor updateVisitor;

        private bool networkEnabled;

        private List<HardwareNode> cpus = new List<HardwareNode>();
        private List<HardwareNode> hdds = new List<HardwareNode>();
        private List<HardwareNode> mainboards = new List<HardwareNode>();

        //private PerformanceCounterCategory PerformanceCounterCategory;
        //private PerformanceCounter performceCounterSent;
        //private PerformanceCounter performceCounterReceived;


        public DynamicHardwareMonitor(PersistentSettings settings, IVisitor updateVisitor)
        {
            this.settings = settings ?? new PersistentSettings();
            this.updateVisitor = updateVisitor;

            unitManager = new UnitManager(settings);

            root = new Node(System.Environment.MachineName);
            
            computer = new Computer(settings);
            LoadMonitorSettings();

            computer.HardwareAdded += new HardwareEventHandler(HardwareAdded);
            computer.HardwareRemoved += new HardwareEventHandler(HardwareRemoved);
            
            computer.Open();
        }

        public Node Root
        {
            get { return root; }
        }

        public IVisitor UpdateVisitor
        {
            set { updateVisitor = value; }
            get { return updateVisitor; }
        }

        public bool EnableGPU
        {
            set
            {
                computer.GPUEnabled = value;
                settings.SetValue(GPU_ENABLE_KEY, value);
            }
            get { return computer.GPUEnabled; }
        }

        public bool EnableCPU
        {
            set
            {
                computer.CPUEnabled = value;
                settings.SetValue(CPU_ENABLE_KEY, value);
            }
            get { return computer.CPUEnabled; }
        }

        public bool EnableRAM
        {
            set
            {
                computer.RAMEnabled = value;
                settings.SetValue(RAM_ENABLE_KEY, value);
            }
            get { return computer.RAMEnabled; }
        }

        public bool EnableMainboard
        {
            set
            {
                computer.MainboardEnabled = value;
                settings.SetValue(MAINBOARD_ENABLE_KEY, value);
            }
            get { return computer.MainboardEnabled; }
        }

        public bool EnableFanController
        {
            set
            {
                computer.FanControllerEnabled = value;
                settings.SetValue(FAN_CONTROLLER_ENABLE_KEY, value);
            }
            get { return computer.FanControllerEnabled; }
        }

        public bool EnableHDD
        {
            set
            {
                computer.HDDEnabled = value;
                settings.SetValue(HDD_ENABLE_KEY, value);
            }
            get { return computer.HDDEnabled; }
        }

        public bool EnableNetwork
        {
            get { return networkEnabled; }
            set { networkEnabled = value; }
        }

        public void Update()
        {
            computer.Accept(updateVisitor);
        }

        public string GetReport()
        {
            return GenerateXML(root).ToString();
        }

        public void Dispose()
        {
            if (computer != null)
                computer.Close();
        }

        public float? GetMaxTemperatureOfCPU()
        {
            return GetMaxValueOfHardwares(cpus, SensorType.Temperature);
        }

        public float? GetMaxTemperatureOfHDD()
        {
            return GetMaxValueOfHardwares(hdds, SensorType.Temperature);
        }

        public float? GetMaxTemperatureOfMainboard()
        {
            return GetMaxValueOfHardwares(mainboards, SensorType.Temperature);
        }

        public float? GetMinTemperatureOfMainboard()
        {
            return GetMinValueOfHardwares(mainboards, SensorType.Temperature);
        }

        private float? GetMaxValueOfHardwares(List<HardwareNode> hardwares, SensorType type)
        {
            float? maxValue = null;
            foreach (HardwareNode hardware in hardwares)
            {
                float? value = GetCurrentValueOfSensor(hardware, type);
                if (value.HasValue)
                {
                    if (!maxValue.HasValue ||
                        maxValue.Value < value.Value)
                    {
                        maxValue = value;
                    }
                }
            }
            return maxValue;
        }

        private float? GetMinValueOfHardwares(List<HardwareNode> hardwares, SensorType type)
        {
            float? minValue = null;
            foreach (HardwareNode hardware in hardwares)
            {
                float? value = GetCurrentValueOfSensor(hardware, type);
                if (value.HasValue)
                {
                    if (!minValue.HasValue ||
                        minValue.Value > value.Value)
                    {
                        minValue = value;
                    }
                }
            }
            return minValue;
        }

        private float? GetCurrentValueOfSensor(HardwareNode hardware, SensorType type)
        {
            float? maxValue = null;
            foreach (Node node in hardware.Nodes)
            {
                TypeNode typeNode = node as TypeNode;
                if (typeNode != null && typeNode.SensorType == type)
                {
                    foreach (Node sensor in  typeNode.Nodes)
                    {
                        SensorNode sensorNode = sensor as SensorNode;
                        float? value = sensorNode.Sensor.Value;
                        if (value.HasValue &&
                            (!maxValue.HasValue || maxValue.Value < value.Value))
                        {
                            maxValue = value;
                        }
                    }
                } else {
                    // subhardware
                    HardwareNode hardwareNode = node as HardwareNode;
                    if (hardwareNode != null)
                    {
                        float? value = GetCurrentValueOfSensor(hardwareNode, type);
                        if (value.HasValue &&
                            (!maxValue.HasValue || maxValue.Value < value.Value))
                        {
                            maxValue = value;
                        }
                    }
                }
            }
            return maxValue;
        }

        private void LoadMonitorSettings()
        {
            computer.CPUEnabled = settings.GetValue(CPU_ENABLE_KEY, true);
            computer.GPUEnabled = settings.GetValue(GPU_ENABLE_KEY, true);
            computer.RAMEnabled = settings.GetValue(RAM_ENABLE_KEY, true);
            computer.HDDEnabled = settings.GetValue(HDD_ENABLE_KEY, true);
            computer.MainboardEnabled = settings.GetValue(MAINBOARD_ENABLE_KEY, true);
            computer.FanControllerEnabled = settings.GetValue(FAN_CONTROLLER_ENABLE_KEY, true);
        }

        private void InsertSorted(Collection<Node> nodes, HardwareNode node)
        {
            int i = 0;
            while (i < nodes.Count && nodes[i] is HardwareNode &&
                   ((HardwareNode)nodes[i]).Hardware.HardwareType <
                     node.Hardware.HardwareType)
                i++;
            nodes.Insert(i, node);
        }

        private void SubHardwareAdded(IHardware hardware, Node node)
        {
            HardwareNode hardwareNode =
              new HardwareNode(hardware, settings, unitManager);

            FindMonitorTarget(hardwareNode);

            InsertSorted(node.Nodes, hardwareNode);

            foreach (IHardware subHardware in hardware.SubHardware)
                SubHardwareAdded(subHardware, hardwareNode);
        }

        private void FindMonitorTarget(HardwareNode node)
        {
            switch (node.Hardware.HardwareType)
            {
                case HardwareType.CPU:
                    cpus.Add(node); break;
                case HardwareType.HDD:
                    hdds.Add(node); break;
                case HardwareType.Mainboard:
                    mainboards.Add(node); break;
                default:
                    break;
            }
        }

        private void HardwareAdded(IHardware hardware)
        {
            SubHardwareAdded(hardware, root);
        }

        private void HardwareRemoved(IHardware hardware)
        {
            List<HardwareNode> nodesToRemove = new List<HardwareNode>();
            foreach (Node node in root.Nodes)
            {
                HardwareNode hardwareNode = node as HardwareNode;
                if (hardwareNode != null && hardwareNode.Hardware == hardware)
                    nodesToRemove.Add(hardwareNode);
            }
            foreach (HardwareNode hardwareNode in nodesToRemove)
            {
                root.Nodes.Remove(hardwareNode);
            }
        }

        private static XElement GenerateXML(Node root)
        {
            XElement rootElement = new XElement("info");
            foreach (Node node in root.Nodes)
            {
                HardwareNode hardwareNode = node as HardwareNode;
                if (hardwareNode != null)
                {
                    rootElement.Add(GenerateHardwareXML(hardwareNode));
                }
            }
            return rootElement;
        }

        private static XElement GenerateHardwareXML(HardwareNode hardwareNode)
        {
            HardwareType type = hardwareNode.Hardware.HardwareType;
            XElement hardwareXML = new XElement(ConvertHardwareTypeToString(type));
            hardwareXML.Add(new XElement("name", hardwareNode.Text));

            foreach (Node node in hardwareNode.Nodes)
            {
                TypeNode typeNode = node as TypeNode;
                if (typeNode != null)
                {
                    hardwareXML.Add(GenerateTypeXML(typeNode));
                }
                else
                {
                    HardwareNode subHardwareNode = node as HardwareNode;
                    if (subHardwareNode != null)
                    {
                        hardwareXML.Add(GenerateHardwareXML(subHardwareNode));
                    }
                }
            }
            return hardwareXML;
        }

        private static XElement GenerateTypeXML(TypeNode typeNode)
        {
            XElement typeElement = new XElement(typeNode.Text);
            foreach (Node node in typeNode.Nodes)
            {
                SensorNode sensorNode = node as SensorNode;
                if (sensorNode != null)
                {
                    typeElement.Add(GenerateSensorXML(sensorNode));
                }
            }
            return typeElement;
        }

        private static XElement GenerateSensorXML(SensorNode sensorNode)
        {
            XElement sensorXML = new XElement("sensor");
            sensorXML.Add(new XAttribute("name", sensorNode.Text));
            sensorXML.Add(new XAttribute("value", sensorNode.Value));
            sensorXML.Add(new XAttribute("min", sensorNode.Min));
            sensorXML.Add(new XAttribute("max", sensorNode.Max));
            return sensorXML;
        }

        private static string ConvertHardwareTypeToString(HardwareType type)
        {
            string result = null;
            switch (type)
            {
                case HardwareType.RAM: result = "ram"; break;
                case HardwareType.CPU: result = "cpu"; break;
                case HardwareType.HDD: result = "hdd"; break;
                case HardwareType.SuperIO: result = "superio"; break;
                case HardwareType.Mainboard: result = "mainboard"; break;
                case HardwareType.Heatmaster:// fall through
                case HardwareType.TBalancer: result = "fan"; break;
                case HardwareType.GpuAti:    // fall through
                case HardwareType.GpuNvidia: result = "gpu"; break;
                default: result = "unknown"; break;
            }
            return result;
        }


    }
}
