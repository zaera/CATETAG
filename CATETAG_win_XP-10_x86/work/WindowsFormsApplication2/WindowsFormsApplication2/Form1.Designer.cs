namespace WindowsFormsApplication2
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatus_SelectedRows = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatus_Separator1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatus_Total = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatus_Separator2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatus_Filter = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatus_ShowAll = new System.Windows.Forms.ToolStripStatusLabel();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            dataGridViewCellStyle1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.dataGridView1.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle2.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.ActiveBorder;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridView1.DefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView1.Location = new System.Drawing.Point(0, 0);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView1.Size = new System.Drawing.Size(579, 317);
            this.dataGridView1.TabIndex = 0;
            this.dataGridView1.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellContentClick);
            this.dataGridView1.DataBindingComplete += new System.Windows.Forms.DataGridViewBindingCompleteEventHandler(this.dataGridView1_DataBindingComplete);
            this.dataGridView1.SelectionChanged += new System.EventHandler(this.dataGridView1_SelectionChanged);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatus_SelectedRows,
            this.toolStripStatus_Separator1,
            this.toolStripStatus_Total,
            this.toolStripStatus_Separator2,
            this.toolStripStatus_Filter,
            this.toolStripStatus_ShowAll});
            this.statusStrip1.Location = new System.Drawing.Point(0, 295);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(579, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatus_SelectedRows
            // 
            this.toolStripStatus_SelectedRows.Name = "toolStripStatus_SelectedRows";
            this.toolStripStatus_SelectedRows.Size = new System.Drawing.Size(81, 17);
            this.toolStripStatus_SelectedRows.Text = "0 row selected";
            // 
            // toolStripStatus_Separator1
            // 
            this.toolStripStatus_Separator1.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.toolStripStatus_Separator1.Name = "toolStripStatus_Separator1";
            this.toolStripStatus_Separator1.Size = new System.Drawing.Size(4, 17);
            // 
            // toolStripStatus_Total
            // 
            this.toolStripStatus_Total.Name = "toolStripStatus_Total";
            this.toolStripStatus_Total.Size = new System.Drawing.Size(89, 17);
            this.toolStripStatus_Total.Text = "Total of number";
            // 
            // toolStripStatus_Separator2
            // 
            this.toolStripStatus_Separator2.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.toolStripStatus_Separator2.Name = "toolStripStatus_Separator2";
            this.toolStripStatus_Separator2.Size = new System.Drawing.Size(4, 17);
            // 
            // toolStripStatus_Filter
            // 
            this.toolStripStatus_Filter.Name = "toolStripStatus_Filter";
            this.toolStripStatus_Filter.Size = new System.Drawing.Size(33, 17);
            this.toolStripStatus_Filter.Text = "Filter";
            // 
            // toolStripStatus_ShowAll
            // 
            this.toolStripStatus_ShowAll.IsLink = true;
            this.toolStripStatus_ShowAll.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
            this.toolStripStatus_ShowAll.Name = "toolStripStatus_ShowAll";
            this.toolStripStatus_ShowAll.Size = new System.Drawing.Size(49, 17);
            this.toolStripStatus_ShowAll.Text = "ShowAll";
            this.toolStripStatus_ShowAll.ToolTipText = "Click to remove the filter";
            this.toolStripStatus_ShowAll.Click += new System.EventHandler(this.toolStripStatus_ShowAll_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(579, 317);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.dataGridView1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatus_SelectedRows;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatus_Separator1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatus_Total;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatus_Separator2;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatus_Filter;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatus_ShowAll;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
    }
}

