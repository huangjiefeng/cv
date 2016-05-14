namespace test
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.btnGetProcessInfo = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.btnGetTerminalInfo = new System.Windows.Forms.Button();
            this.btnGetHardwareInfo = new System.Windows.Forms.Button();
            this.btnShutDown = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnGetProcessInfo
            // 
            this.btnGetProcessInfo.Location = new System.Drawing.Point(12, 12);
            this.btnGetProcessInfo.Name = "btnGetProcessInfo";
            this.btnGetProcessInfo.Size = new System.Drawing.Size(168, 23);
            this.btnGetProcessInfo.TabIndex = 0;
            this.btnGetProcessInfo.Text = "GetProcessInfo";
            this.btnGetProcessInfo.UseVisualStyleBackColor = true;
            this.btnGetProcessInfo.Click += new System.EventHandler(this.btnGetProcessInfo_Click);
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox1.Location = new System.Drawing.Point(186, 12);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox1.Size = new System.Drawing.Size(778, 487);
            this.textBox1.TabIndex = 1;
            // 
            // btnGetTerminalInfo
            // 
            this.btnGetTerminalInfo.Location = new System.Drawing.Point(12, 41);
            this.btnGetTerminalInfo.Name = "btnGetTerminalInfo";
            this.btnGetTerminalInfo.Size = new System.Drawing.Size(168, 23);
            this.btnGetTerminalInfo.TabIndex = 2;
            this.btnGetTerminalInfo.Text = "GetTerminalInfo";
            this.btnGetTerminalInfo.UseVisualStyleBackColor = true;
            this.btnGetTerminalInfo.Click += new System.EventHandler(this.btnGetTerminalInfo_Click);
            // 
            // btnGetHardwareInfo
            // 
            this.btnGetHardwareInfo.Location = new System.Drawing.Point(12, 70);
            this.btnGetHardwareInfo.Name = "btnGetHardwareInfo";
            this.btnGetHardwareInfo.Size = new System.Drawing.Size(168, 23);
            this.btnGetHardwareInfo.TabIndex = 3;
            this.btnGetHardwareInfo.Text = "GetHardwareInfo";
            this.btnGetHardwareInfo.UseVisualStyleBackColor = true;
            this.btnGetHardwareInfo.Click += new System.EventHandler(this.btnGetHardwareInfo_Click);
            // 
            // btnShutDown
            // 
            this.btnShutDown.Location = new System.Drawing.Point(12, 99);
            this.btnShutDown.Name = "btnShutDown";
            this.btnShutDown.Size = new System.Drawing.Size(168, 23);
            this.btnShutDown.TabIndex = 3;
            this.btnShutDown.Text = "ShutDown";
            this.btnShutDown.UseVisualStyleBackColor = true;
            this.btnShutDown.Click += new System.EventHandler(this.btnShutDown_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(976, 511);
            this.Controls.Add(this.btnShutDown);
            this.Controls.Add(this.btnGetHardwareInfo);
            this.Controls.Add(this.btnGetTerminalInfo);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.btnGetProcessInfo);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnGetProcessInfo;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button btnGetTerminalInfo;
        private System.Windows.Forms.Button btnGetHardwareInfo;
        private System.Windows.Forms.Button btnShutDown;
    }
}

