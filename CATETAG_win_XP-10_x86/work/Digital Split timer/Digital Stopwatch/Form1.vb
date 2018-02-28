'Description: This porgram is a Digital Stopwatch that ables the user to start the timer,stop, pause, reset and continue.
'Author:    Joken Villanueva
'Date Created: N/A
Public Class Form1

    Dim a As String
    Dim b As String
    Dim c As String
    Dim d As String
    Dim y As String
    Dim z As String
    Dim h As String
    Dim m As String
    Dim s As String
    Dim u As String
    Dim v As String
    Dim DT As New DataTable
    Sub runningtime()
        If Val(v) < 6 Then
            v = v + 1
            txttime.Text = a + b + ":" + c + d + ":" + y + z + "." + u + v
            'check if u is less then 9
        ElseIf Val(u) < 9 Then
            v = 0
            'incretment u by 1
            u = u + 1
            'display for miliseconds
            txttime.Text = a + b + ":" + c + d + ":" + y + z + "." + u + v
            'check if z is still less than 9
        ElseIf Val(z) < 9 Then
            'set the milisecond to 0
            v = 0
            u = 0
            'increment
            z = z + 1
            'display for second(s)
            txttime.Text = a + b + ":" + c + d + ":" + y + z + "." + u + v
        ElseIf Val(y) < 5 Then
            v = 0
            u = 0
            z = 0
            y = y + 1
            txttime.Text = a + b + ":" + c + d + ":" + y + z + "." + u + v
        ElseIf Val(d) < 9 Then
            v = 0
            u = 0
            z = 0
            y = 0
            d = d + 1
            txttime.Text = a + b + ":" + c + d + ":" + y + z + "." + u + v
        ElseIf Val(c) < 5 Then
            v = 0
            u = 0
            z = 0
            y = 0
            d = 0
            c = c + 1
            txttime.Text = a + b + ":" + c + d + ":" + y + z + "." + u + v
        ElseIf Val(b) < 9 Then
            v = 0
            u = 0
            z = 0
            y = 0
            d = 0
            c = 0
            b = b + 1
            txttime.Text = a + b + ":" + c + d + ":" + y + z + "." + u + v
        ElseIf Val(b) < 9 Then
            v = 0
            u = 0
            z = 0
            y = 0
            d = 0
            c = 0
            b = b + 1
            txttime.Text = a + b + ":" + c + d + ":" + y + z + "." + u + v
        ElseIf Val(a) < 9 Then
            v = 0
            u = 0
            z = 0
            y = 0
            d = 0
            c = 0
            b = 0
            a = a + 1
            'this is the final output if reach the hour value
            txttime.Text = a + b + ":" + c + d + ":" + y + z + "." + u + v

            '1 sec = 1000ms
            '1 min = 60000ms
            '1 hr = 36000000ms


        End If

    End Sub
    Sub restart()

        a = "0"
        b = "0"
        c = "0"
        d = "0"
        y = "0"
        z = "0"
        u = "0"
        v = "0"

        h = a + b
        m = c + d
        s = y + z


    End Sub
    Sub stoptime()
        Timer1.Enabled = False
        a = "0"
        b = "0"
        c = "0"
        d = "0"
        y = "0"
        z = "0"
        u = "0"
        v = "0"

        h = a + b
        m = c + d
        s = y + z
        'display as "00:00:00.00"
        txttime.Text = h + ":" + m + ":" + s + "." + u + v
        BtnStart.Enabled = True

    End Sub
    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        'this single code, it simply call the runningtime() sub procedure
        runningtime()
    End Sub



    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        'it do the initialization
        'add new column for our datagridview
        DT.Columns.Add("Split Time Record")
        'get the datasource from Datatable named DT
        DataGridView1.DataSource = DT
        'set the sort mode of the datagrid into descending order.
        DataGridView1.Sort(DataGridView1.Columns(0), System.ComponentModel.ListSortDirection.Descending)


        a = "0"
        b = "0"
        c = "0"
        d = "0"
        y = "0"
        z = "0"
        u = "0"
        v = "0"

        h = a + b
        m = c + d
        s = y + z
        'display as "00:00:00.00"
        txttime.Text = h + ":" + m + ":" + s + "." + u + v
    End Sub

    Private Sub btnstop_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnstop.Click
        stoptime()
    End Sub

    Private Sub btnrestart_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnrestart.Click
        restart()
    End Sub

    Private Sub Button1_Click_1(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Timer1.Enabled = False
        BtnStart.Enabled = True
        BtnStart.Text = "Continue"
    End Sub

    Private Sub btnexit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Me.Close()
    End Sub

    Private Sub BtnStart_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtnStart.Click
        'start the timer
        Timer1.Start()
        'check if the text of these button is set to continue set by pause button then it set to start
        If BtnStart.Text = "Start" Then
            BtnStart.Enabled = False
        End If

    End Sub
   
    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnsplit.Click
        DataGridView1.Visible = True
        DT.Rows.Add(txttime.Text)
    End Sub
End Class
