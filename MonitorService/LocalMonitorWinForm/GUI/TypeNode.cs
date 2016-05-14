/*
 
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
  Copyright (C) 2009-2012 Michael Möller <mmoeller@openhardwaremonitor.org>
	
*/

using System;
using System.Collections.Generic;
using OpenHardwareMonitor.Hardware;

namespace OpenHardwareMonitor.GUI {
  public class TypeNode : Node {

    private SensorType sensorType;

    public TypeNode(SensorType sensorType) : base() {
      this.sensorType = sensorType;

      switch (sensorType) {
        case SensorType.Voltage: 
          this.Text = "voltages";
          break;
        case SensorType.Clock:
          this.Text = "clocks";
          break;
        case SensorType.Load:
          this.Text = "load";
          break;
        case SensorType.Temperature:
          this.Text = "temperatures";
          break;
        case SensorType.Fan:
          this.Text = "fans";
          break;
        case SensorType.Flow:
          this.Text = "flows";
          break;
        case SensorType.Control:
          this.Text = "controls";
          break;
        case SensorType.Level:
          this.Text = "levels";
          break;
        case SensorType.Power:
          this.Text = "powers";
          break;
        case SensorType.Data:
          this.Text = "data";
          break;
        case SensorType.Factor:
          this.Text = "factors";
          break;
      }

      NodeAdded += new NodeEventHandler(TypeNode_NodeAdded);
      NodeRemoved += new NodeEventHandler(TypeNode_NodeRemoved);
    }

    private void TypeNode_NodeRemoved(Node node) {
      node.IsVisibleChanged -= new NodeEventHandler(node_IsVisibleChanged);
      node_IsVisibleChanged(null);
    }    

    private void TypeNode_NodeAdded(Node node) {
      node.IsVisibleChanged += new NodeEventHandler(node_IsVisibleChanged);
      node_IsVisibleChanged(null);
    }

    private void node_IsVisibleChanged(Node node) {      
      foreach (Node n in Nodes)
        if (n.IsVisible) {
          this.IsVisible = true;
          return;
        }
      this.IsVisible = false;
    }

    public SensorType SensorType {
      get { return sensorType; }
    }
  }
}
