using System;
using System.Linq;
using System.Text;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace OpenHardwareMonitor.GUI
{
    public class Node
    {
        private Node parent;
        private NodeCollection nodes;

        private string text;
        private bool visible;

        public Node() : this(string.Empty) { }

        public Node(string text)
        {
            this.text = text;
            this.nodes = new NodeCollection(this);
            this.visible = true;
        }

        public Node Parent
        {
            get { return parent; }
            set
            {
                if (value != parent)
                {
                    if (parent != null)
                        parent.nodes.Remove(this);
                    if (value != null)
                        value.nodes.Add(this);
                }
            }
        }

        public Collection<Node> Nodes
        {
            get { return nodes; }
        }

        public virtual string Text
        {
            get { return text; }
            set
            {
                if (text != value)
                {
                    text = value;
                }
            }
        }

        public virtual bool IsVisible
        {
            get { return visible; }
            set
            {
                if (value != visible)
                {
                    visible = value;
                    if (parent != null)
                    {
                        int index = 0;
                        for (int i = 0; i < parent.nodes.Count; i++)
                        {
                            Node node = parent.nodes[i];
                            if (node == this)
                                break;
                            if (node.IsVisible)
                                index++;
                        }
                    }
                    if (IsVisibleChanged != null)
                        IsVisibleChanged(this);
                }
            }
        }

        public delegate void NodeEventHandler(Node node);

        public event NodeEventHandler IsVisibleChanged;
        public event NodeEventHandler NodeAdded;
        public event NodeEventHandler NodeRemoved;

        private class NodeCollection : Collection<Node>
        {
            private Node owner;

            public NodeCollection(Node owner)
            {
                this.owner = owner;
            }

            protected override void ClearItems()
            {
                while (this.Count != 0)
                    this.RemoveAt(this.Count - 1);
            }

            protected override void InsertItem(int index, Node item)
            {
                if (item == null)
                    throw new ArgumentNullException("item");

                if (item.parent != owner)
                {
                    if (item.parent != null)
                        item.parent.nodes.Remove(item);
                    item.parent = owner;
                    base.InsertItem(index, item);

                    if (owner.NodeAdded != null)
                        owner.NodeAdded(item);
                }
            }

            protected override void RemoveItem(int index)
            {
                Node item = this[index];
                item.parent = null;
                base.RemoveItem(index);

                if (owner.NodeRemoved != null)
                    owner.NodeRemoved(item);
            }

            protected override void SetItem(int index, Node item)
            {
                if (item == null)
                    throw new ArgumentNullException("item");

                RemoveAt(index);
                InsertItem(index, item);
            }
        }
    }
}
