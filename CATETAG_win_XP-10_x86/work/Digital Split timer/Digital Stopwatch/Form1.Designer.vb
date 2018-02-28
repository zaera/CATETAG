<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.BtnStart = New System.Windows.Forms.Button()
        Me.btnstop = New System.Windows.Forms.Button()
        Me.btnrestart = New System.Windows.Forms.Button()
        Me.txttime = New System.Windows.Forms.TextBox()
        Me.Button1 = New System.Windows.Forms.Button()
        Me.DataGridView1 = New System.Windows.Forms.DataGridView()
        Me.btnsplit = New System.Windows.Forms.Button()
        CType(Me.DataGridView1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'Timer1
        '
        Me.Timer1.Interval = 1
        '
        'BtnStart
        '
        Me.BtnStart.BackColor = System.Drawing.SystemColors.AppWorkspace
        Me.BtnStart.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.BtnStart.Font = New System.Drawing.Font("Calibri", 14.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(204, Byte))
        Me.BtnStart.ForeColor = System.Drawing.SystemColors.ActiveBorder
        Me.BtnStart.Location = New System.Drawing.Point(0, 78)
        Me.BtnStart.Name = "BtnStart"
        Me.BtnStart.Size = New System.Drawing.Size(95, 31)
        Me.BtnStart.TabIndex = 1
        Me.BtnStart.Text = "Start"
        Me.BtnStart.UseVisualStyleBackColor = False
        '
        'btnstop
        '
        Me.btnstop.BackColor = System.Drawing.SystemColors.AppWorkspace
        Me.btnstop.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.btnstop.Font = New System.Drawing.Font("Calibri", 14.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(204, Byte))
        Me.btnstop.ForeColor = System.Drawing.SystemColors.ActiveBorder
        Me.btnstop.Location = New System.Drawing.Point(55, 0)
        Me.btnstop.Name = "btnstop"
        Me.btnstop.Size = New System.Drawing.Size(63, 31)
        Me.btnstop.TabIndex = 2
        Me.btnstop.Text = "Stop"
        Me.btnstop.UseVisualStyleBackColor = False
        '
        'btnrestart
        '
        Me.btnrestart.BackColor = System.Drawing.SystemColors.AppWorkspace
        Me.btnrestart.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.btnrestart.Font = New System.Drawing.Font("Calibri", 14.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(204, Byte))
        Me.btnrestart.ForeColor = System.Drawing.SystemColors.ActiveBorder
        Me.btnrestart.Location = New System.Drawing.Point(124, 0)
        Me.btnrestart.Name = "btnrestart"
        Me.btnrestart.Size = New System.Drawing.Size(69, 31)
        Me.btnrestart.TabIndex = 3
        Me.btnrestart.Text = "Reset"
        Me.btnrestart.UseVisualStyleBackColor = False
        '
        'txttime
        '
        Me.txttime.BackColor = System.Drawing.Color.MintCream
        Me.txttime.Font = New System.Drawing.Font("Calibri", 20.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(204, Byte))
        Me.txttime.Location = New System.Drawing.Point(-1, 37)
        Me.txttime.Name = "txttime"
        Me.txttime.Size = New System.Drawing.Size(194, 40)
        Me.txttime.TabIndex = 4
        Me.txttime.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Button1
        '
        Me.Button1.BackColor = System.Drawing.SystemColors.AppWorkspace
        Me.Button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.Button1.Font = New System.Drawing.Font("Calibri", 14.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(204, Byte))
        Me.Button1.ForeColor = System.Drawing.SystemColors.ActiveBorder
        Me.Button1.Location = New System.Drawing.Point(101, 78)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(92, 31)
        Me.Button1.TabIndex = 5
        Me.Button1.Text = "Pause"
        Me.Button1.UseVisualStyleBackColor = False
        '
        'DataGridView1
        '
        Me.DataGridView1.AllowUserToAddRows = False
        Me.DataGridView1.AllowUserToOrderColumns = True
        Me.DataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill
        Me.DataGridView1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.DataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize
        Me.DataGridView1.Location = New System.Drawing.Point(0, 115)
        Me.DataGridView1.Name = "DataGridView1"
        Me.DataGridView1.RowHeadersVisible = False
        Me.DataGridView1.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing
        Me.DataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect
        Me.DataGridView1.Size = New System.Drawing.Size(193, 175)
        Me.DataGridView1.TabIndex = 8
        '
        'btnsplit
        '
        Me.btnsplit.BackColor = System.Drawing.SystemColors.AppWorkspace
        Me.btnsplit.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.btnsplit.Font = New System.Drawing.Font("Calibri", 14.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(204, Byte))
        Me.btnsplit.ForeColor = System.Drawing.SystemColors.ActiveBorder
        Me.btnsplit.Location = New System.Drawing.Point(0, 0)
        Me.btnsplit.Name = "btnsplit"
        Me.btnsplit.Size = New System.Drawing.Size(49, 31)
        Me.btnsplit.TabIndex = 9
        Me.btnsplit.Text = "Lap"
        Me.btnsplit.UseVisualStyleBackColor = False
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackColor = System.Drawing.SystemColors.ControlDarkDark
        Me.ClientSize = New System.Drawing.Size(195, 291)
        Me.Controls.Add(Me.btnsplit)
        Me.Controls.Add(Me.DataGridView1)
        Me.Controls.Add(Me.Button1)
        Me.Controls.Add(Me.txttime)
        Me.Controls.Add(Me.btnrestart)
        Me.Controls.Add(Me.btnstop)
        Me.Controls.Add(Me.BtnStart)
        Me.ForeColor = System.Drawing.SystemColors.ControlDarkDark
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.Name = "Form1"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        CType(Me.DataGridView1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents Timer1 As System.Windows.Forms.Timer
    Friend WithEvents BtnStart As System.Windows.Forms.Button
    Friend WithEvents btnstop As System.Windows.Forms.Button
    Friend WithEvents btnrestart As System.Windows.Forms.Button
    Friend WithEvents txttime As System.Windows.Forms.TextBox
    Friend WithEvents Button1 As System.Windows.Forms.Button
    Friend WithEvents DataGridView1 As System.Windows.Forms.DataGridView
    Friend WithEvents btnsplit As System.Windows.Forms.Button

End Class
