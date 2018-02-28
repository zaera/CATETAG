using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Configuration;
using System.Media;
using System.Threading;
using System.Linq;
using System.IO.Ports;
using DataGridViewAutoFilter;
using Excel = Microsoft.Office.Interop.Excel;
using System.Reflection;
using ADOX;
using System.Data.SqlClient;
using System.Xml;
//using System;
using System.Diagnostics;
//using System.Threading;
namespace SimpleSerial
{
    public partial class compet : Form
    {

        public class TransparentPanel : System.Windows.Forms.Panel
        {
            [Browsable(false)]
            protected override CreateParams CreateParams
            {
                get
                {
                    CreateParams cp = base.CreateParams;
                    cp.ExStyle |= 0x20;
                    return cp;
                }
            }
            protected override void OnPaintBackground(PaintEventArgs e)
            {
                // Do Nothing
            }
        }
        // Add this variable 
        string RxString;
        int go = 0;

        public compet()
        {
            InitializeComponent();
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            serialPort1.PortName = "COM3";
            serialPort1.BaudRate = 115200;

            serialPort1.Open();

        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Close();


            }

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen) serialPort1.Close();
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            // If the port is closed, don't try to send a character.
            if (!serialPort1.IsOpen) return;

            // If the port is Open, declare a char[] array with one element.
            char[] buff = new char[1];

            // Load element 0 with the key character.
            buff[0] = e.KeyChar;

            // Send the one character buffer.
            serialPort1.Write(buff, 0, 1);

            // Set the KeyPress event as handled so the character won't
            // display locally. If you want it to display, omit the next line.
            e.Handled = true;
        }

        private void DisplayText(object sender, EventArgs e)
        {
            if (go == 0)
            {
                textBox1.AppendText(RxString);
                addtodatagrid();
            }

            else if (go == 1)
            {
                textBox1.AppendText(RxString);
                addtodatagrid2();
            }
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            RxString = serialPort1.ReadExisting();
            this.Invoke(new EventHandler(DisplayText));
        }
        int nn = 0;














        private int recheck2(int iii)
        {
            string uidd = "$$$$$$$$";
            for (int i = 0; i < dataGridView2.RowCount; i++)
                if (dataGridView2[0, i].FormattedValue.ToString().
                    Contains(uidd.ToString()))
                {
                    dataGridView2.ClearSelection();
                    dataGridView2[0, i].Selected = true;
                    iii = i;
                } return iii;
        }
        //-------------------------------------------------------------------------------------------------------------------------
        private void check2(string uid)
        {
            string uidd = uid;
            for (int i = 0; i < dataGridView2.RowCount; i++)
                if (dataGridView2[0, i].FormattedValue.ToString().
                    Contains(uidd.ToString()))
                {
                    Console.Beep(800, 100);
                    Console.Beep(800, 100);
                    p1.BackColor = Color.Salmon;
                    p2.BackColor = Color.Salmon;
                    p3.BackColor = Color.Salmon;
                    p4.BackColor = Color.Salmon;
                    timer2.Interval = 1000;
                    timer2.Start();
                    textBox1.Text = "";
                    MessageBox.Show(uidd + " - already exist! Please delete it to write again.");
                    dataGridView2.ClearSelection();
                    dataGridView2[0, i].Selected = true;
                 //  nnn = 1;
                    return;
                }
        }
        //-------------------------------------------------------------------------------------------------------------------------
        private void addtodatagrid2()
        {
            textBox1.ReadOnly = true;
            string a = textBox1.Text;
            String c = "***";
            int l = a.Length;
            try
            {
                if (textBox1.Text == "") { return; }
                else if (a.Contains("r") == true || a.Contains("i") == true || a.Contains("u") == true || a.Contains(".") == true)
                {
                    // MessageBox.Show("MIFARE Read failed!!!!");
                    Console.Beep(800, 100);
                    Console.Beep(800, 100);
                    p1.BackColor = Color.Salmon;
                    p2.BackColor = Color.Salmon;
                    p3.BackColor = Color.Salmon;
                    p4.BackColor = Color.Salmon;
                    timer2.Interval = 1000;
                    timer2.Start();
                    textBox1.Text = "";
                    return;
                    //label1.Text = "";
                }

                else if ((l - 3) > 0 && a.Substring(l - 3, 3) == c && l > 13 && a != "")
                {
                    a = a.Replace(" ", "");
                    int aa = a.Length;
                    a = a.Substring(0, aa - 3);
                    aa = a.Length;
                    textBox1.Text = "";
                    String uid = a.Substring(0, 8);
                    check2(uid);
                    if (nn == 0)
                    {
                        dataGridView2.Rows.Add("$$$$$$$$");
                        int row = recheck2(0);
                        dataGridView2.Rows[row].Cells[0].Value = uid;

                        Console.Beep(3000, 300);
                        p1.BackColor = Color.MediumAquamarine;
                        p2.BackColor = Color.MediumAquamarine;
                        p3.BackColor = Color.MediumAquamarine;
                        p4.BackColor = Color.MediumAquamarine;
                        timer2.Interval = 1000;
                        timer2.Start();
                        textBox1.Text = "";
                        //label1.Text = "";
                        try { resyncregister(); }
                        catch { return; }




                        return;
                    }
                    else { nn = 0; return; }

                }
                try { resyncregister(); }
                catch { return; }


            }
            catch
            {
                MessageBox.Show("Read failed!");








                return;
            }

            return;
        }









        private int recheck(int ii)
        {
            string uidd = "$$$$$$$$";

            for (int i = 0; i < dataGridView1.RowCount; i++)
                if (dataGridView1[0, i].FormattedValue.ToString().
                    Contains(uidd.ToString()))
                {

                    dataGridView1.ClearSelection();


                    dataGridView1[0, i].Selected = true;
                    ii = i;
                }
            return ii;

        }


        private void check(string uid)
        {
            string uidd = uid;

            for (int i = 0; i < dataGridView1.RowCount; i++)
                if (dataGridView1[0, i].FormattedValue.ToString().
                    Contains(uidd.ToString()))
                {

                    Console.Beep(800, 100);
                    Console.Beep(800, 100);
                    p1.BackColor = Color.Salmon;
                    p2.BackColor = Color.Salmon;
                    p3.BackColor = Color.Salmon;
                    p4.BackColor = Color.Salmon;
                    timer2.Interval = 1000;
                    timer2.Start();
                    textBox1.Text = "";
                    // label1.Text = "";


                    MessageBox.Show(uidd + " - already exist! Please delete it to write again.");


                    dataGridView1.ClearSelection();


                    dataGridView1[0, i].Selected = true;

                    nn = 1;


                    return;

                }

        }






        private void addtodatagrid()
        {
            textBox1.ReadOnly = true;
            string a = textBox1.Text;
            String c = "***";
            int l = a.Length;


            try
            {

                if (textBox1.Text == "") { return; }


                else if (a.Contains("r") == true || a.Contains("i") == true || a.Contains("u") == true || a.Contains(".") == true)
                {
                    // MessageBox.Show("MIFARE Read failed!!!!");
                    Console.Beep(800, 100);
                    Console.Beep(800, 100);
                    p1.BackColor = Color.Salmon;
                    p2.BackColor = Color.Salmon;
                    p3.BackColor = Color.Salmon;
                    p4.BackColor = Color.Salmon;
                    timer2.Interval = 1000;
                    timer2.Start();
                    textBox1.Text = "";
                    return;
                    //label1.Text = "";
                }

                else if ((l - 3) > 0 && a.Substring(l - 3, 3) == c && l > 13 && a != "")
                {



                    // row = 0;

                    a = a.Replace(" ", "");

                    int aa = a.Length;

                    a = a.Substring(0, aa - 3);

                    aa = a.Length;


                    textBox1.Text = "";


                    String uid = a.Substring(0, 8);
                    check(uid);

                    if (nn == 0)
                    {
                        // MessageBox.Show("cyka1");
                        dataGridView1.Rows.Add("$$$$$$$$");
                        // MessageBox.Show("cyka2");


                        int row = recheck(0);
                        // MessageBox.Show(row.ToString());

                        //return;








                        String cpt = a.Substring(8, aa - 8);


                        aa = aa - 8;
                        int b = (aa / 10);
                        int numberofcp = b * 4;


                        dataGridView1.Rows[row].Cells[0].Value = uid;

                        dataGridView1.Rows[row].Cells[1].Value = " ";
                        dataGridView1.Rows[row].Cells[4].Value = 00;
                        dataGridView1.Rows[row].Cells[5].Value = 00;
                        dataGridView1.Rows[row].Cells[7].Value = " ";
                        dataGridView1.Rows[row].Cells[8].Value = " ";
                        dataGridView1.Rows[row].Cells[9].Value = " ";
                        dataGridView1.Rows[row].Cells[10].Value = " ";


                        int cptimebuffer = 0;

                        //MessageBox.Show(column.ToString()); //11
                        // MessageBox.Show(b.ToString());      //6


                        // return;
                        int column = dataGridView1.ColumnCount - 11;
                        // MessageBox.Show(column.ToString());
                        if (numberofcp > column)
                        {

                            for (int zz = 0; zz < ((numberofcp - column) / 4); zz++)
                            {

                                /*    dataGridView1.Columns.Add("", "CP");
                                    dataGridView1.Columns.Add("", "Time");
                                    dataGridView1.Columns.Add("", "Lap");
                                    dataGridView1.Columns.Add("", "Lap.ms"); */
                                int x = (column / 4) + zz;
                                //      MessageBox.Show(x.ToString());
                                dataGridView1.Columns.Add("", String.Join("", "CP", x.ToString()));
                                dataGridView1.Columns.Add("", String.Join("", "Time", x.ToString()));
                                dataGridView1.Columns.Add("", String.Join("", "Lap", x.ToString()));
                                dataGridView1.Columns.Add("", String.Join("", "Lap(ms)", x.ToString()));
                            }
                        }





                        for (int i = 0; i < b; i++)
                        {

                            string cp = cpt.Substring(i * 10, 3);
                            //MessageBox.Show(cp);
                            string time = cpt.Substring(3 + (i * 10), 7);
                            //MessageBox.Show(time);

                            if (int.Parse(cp) == 204)
                            {
                                dataGridView1.Rows[row].Cells[8 + 3 + i * 4].Value = "START";

                                dataGridView1.Rows[row].Cells[6].Value = "S";
                                int t1 = int.Parse(time);
                                dataGridView1.Rows[row].Cells[4].Value = t1;
                            }
                            else if (int.Parse(cp) == 255)
                            {
                                dataGridView1.Rows[row].Cells[8 + 3 + i * 4].Value = "FINISH";
                                string seq = dataGridView1.Rows[row].Cells[6].Value.ToString();
                                seq = String.Join("-", seq, "F");
                                dataGridView1.Rows[row].Cells[6].Value = seq;

                                int t2 = int.Parse(time);
                                dataGridView1.Rows[row].Cells[5].Value = t2;
                            }
                            else
                            {
                                int cpnorm = int.Parse(cp);
                                cpnorm = cpnorm - 70;
                                dataGridView1.Rows[row].Cells[8 + 3 + i * 4].Value = cpnorm.ToString();
                                try
                                {
                                    string seq = dataGridView1.Rows[row].Cells[6].Value.ToString();

                                    seq = String.Join("-", seq, cpnorm.ToString()); dataGridView1.Rows[row].Cells[6].Value = seq;
                                }
                                catch { }
                            }
                            long starttime = int.Parse(dataGridView1.Rows[row].Cells[4].Value.ToString());
                            long finishtime = int.Parse(dataGridView1.Rows[row].Cells[5].Value.ToString());
                            long runtimems = finishtime - starttime;
                            dataGridView1.Rows[row].Cells[2].Value = runtimems;

                            int t = int.Parse(dataGridView1.Rows[row].Cells[2].Value.ToString());


                            int h = t / 360000;
                            int m = (t - (h * 360000)) / 6000;
                            int s = (t - (h * 360000) - (m * 6000)) / 100;
                            int ms = t - (h * 360000) - (m * 6000) - (s * 100);
                            string hms;
                            string mms;
                            if (ms < 10)
                            {
                                mms = String.Join("", "0", ms);
                                hms = String.Join(":", h, m, s, mms);
                            }
                            else { hms = String.Join(":", h, m, s, ms); }
                            dataGridView1.Rows[row].Cells[3].Value = hms;

                            try
                            {

                                t = int.Parse(time);
                            }
                            catch { }
                            h = t / 360000;
                            m = (t - (h * 360000)) / 6000;
                            s = (t - (h * 360000) - (m * 6000)) / 100;
                            ms = t - (h * 360000) - (m * 6000) - (s * 100);
                            if (ms < 10)
                            {
                                mms = String.Join("", "0", ms);
                                hms = String.Join(":", h, m, s, mms);
                            }
                            else { hms = String.Join(":", h, m, s, ms); }

                            int lapms = int.Parse(time) - (cptimebuffer);
                            cptimebuffer = int.Parse(time);

                            string lap;

                            t = lapms;
                            h = t / 360000;
                            m = (t - (h * 360000)) / 6000;
                            s = (t - (h * 360000) - (m * 6000)) / 100;
                            ms = t - (h * 360000) - (m * 6000) - (s * 100);
                            if (ms < 10)
                            {
                                mms = String.Join("", "0", ms);
                                lap = String.Join(":", h, m, s, mms);
                            }
                            else { lap = String.Join(":", h, m, s, ms); }

                            dataGridView1.Rows[row].Cells[9 + 3 + i * 4].Value = hms;
                            dataGridView1.Rows[row].Cells[10 + 3 + i * 4].Value = lap;
                            dataGridView1.Rows[row].Cells[11 + 3 + i * 4].Value = lapms;
                        }
                        int t3 = int.Parse(dataGridView1.Rows[row].Cells[4].Value.ToString());
                        int h3 = t3 / 360000;
                        int m3 = (t3 - (h3 * 360000)) / 6000;
                        int s3 = (t3 - (h3 * 360000) - (m3 * 6000)) / 100;
                        int ms3 = t3 - (h3 * 360000) - (m3 * 6000) - (s3 * 100);
                        string hms3 = String.Join(":", h3, m3, s3, ms3);
                        string mms3;
                        if (ms3 < 10)
                        {
                            mms3 = String.Join("", "0", ms3);
                            hms3 = String.Join(":", h3, m3, s3, mms3);
                        }
                        else { hms3 = String.Join(":", h3, m3, s3, ms3); }
                        dataGridView1.Rows[row].Cells[4].Value = hms3;

                        t3 = int.Parse(dataGridView1.Rows[row].Cells[5].Value.ToString());
                        h3 = t3 / 360000;
                        m3 = (t3 - (h3 * 360000)) / 6000;
                        s3 = (t3 - (h3 * 360000) - (m3 * 6000)) / 100;
                        ms3 = t3 - (h3 * 360000) - (m3 * 6000) - (s3 * 100);
                        if (ms3 < 10)
                        {
                            mms3 = String.Join("", "0", ms3);
                            hms3 = String.Join(":", h3, m3, s3, mms3);
                        }
                        else { hms3 = String.Join(":", h3, m3, s3, ms3); }
                        dataGridView1.Rows[row].Cells[5].Value = hms3;
                        dataGridView1.Rows[row].Cells[10 + 3].Value = "0";
                        dataGridView1.Rows[row].Cells[11 + 3].Value = "0";
                        //dataGridView1.Rows.Insert(0);
                        //columncount = 0;
                        Console.Beep(3000, 300);
                        p1.BackColor = Color.MediumAquamarine;
                        p2.BackColor = Color.MediumAquamarine;
                        p3.BackColor = Color.MediumAquamarine;
                        p4.BackColor = Color.MediumAquamarine;
                        timer2.Interval = 1000;
                        timer2.Start();
                        textBox1.Text = "";
                        //label1.Text = "";
                        try { resyncregister(); }
                        catch { return; }




                        return;
                    }
                    else { nn = 0; return; }

                }
                try { resyncregister(); }
                catch { return; }


            }
            catch
            {
                MessageBox.Show("Read failed!");





                //     dataGridView1.Rows.RemoveAt(o2);


                return;
            }
            int tttt = dataGridView1.ColumnCount;
            for (int y = 11; y < tttt; y++)
            {
                this.dataGridView1.Columns[y].Visible = false;
            }
            return;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Start();
            ComboBox1.ForeColor = Color.Salmon;
            // Get all serial ports name
            string[] ports = SerialPort.GetPortNames();

            // Clear comboBox list to get latest serial ports in each dropdown.
            ComboBox1.Items.Clear();
            ComboBox1.Items.Add("Select!");
            foreach (string port in ports)
            {
                // Output serial port to dropdown list.

                ComboBox1.Items.Add(port);


            }
            ComboBox1.SelectedIndex = 0;

            int t = dataGridView1.ColumnCount;
            for (int y = 11; y < t; y++)
            {
                this.dataGridView1.Columns[y].Visible = false;
            }
        }
        int hl = 0;
        private void button1_Click(object sender, EventArgs e)
        {
            int t = dataGridView1.ColumnCount;
            if (hl == 0)
            {
                hl = 1;
                for (int y = 11; y < t; y++)
                {
                    this.dataGridView1.Columns[y].Visible = false;
                }

            }
            else if (hl == 1)
            {
                hl = 0;
                for (int y = 11; y < t; y++)
                {
                    this.dataGridView1.Columns[y].Visible = true;
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            int t = dataGridView1.ColumnCount;
            for (int y = 11; y < t; y++)
            {
                this.dataGridView1.Columns[y].Visible = true;
            }

        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
        int k = 0;
        private void button8_Click(object sender, EventArgs e)
        {
            if (k == 0)
            {
                panel3.Visible = true;
                k = k + 1;
                dataGridView1.Size = new Size(1005, 243);
            }
            else if (k == 1)
            {
                //panel3.Visible = false;
                k = k + 1;
                panel3.Size = new Size(1005, 582);
                panel3.Location = new Point(10, 9);
                dataGridView2.Size = new Size(1004, 549);


            }
            else if (k == 2)
            {
                panel3.Size = new Size(1006, 310);
                panel3.Location = new Point(10, 280);
                panel3.Visible = false;
                k = 0;
                dataGridView2.Size = new Size(1004, 278);
                dataGridView1.Size = new Size(1005, 554);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            dataGridView1.DataSource = null;
            dataGridView1.Refresh();
            dataGridView1.Columns.Clear();

            dataGridView1.Columns.Add("", "UID");
            dataGridView1.Columns.Add("", "NAME");
            dataGridView1.Columns.Add("", "RT(ms)");
            dataGridView1.Columns.Add("", "RT");
            dataGridView1.Columns.Add("", "START");
            dataGridView1.Columns.Add("", "FINISH");
            dataGridView1.Columns.Add("", "SEQUENCE");
            dataGridView1.Columns.Add("", "CAT");
            dataGridView1.Columns.Add("", "COACH");
            dataGridView1.Columns.Add("", "COMM");
            dataGridView1.Columns.Add("", "CLUB");
            dataGridView1.Rows.Clear();

            dataGridView1.Refresh();


        }


        private void button4_Click(object sender, EventArgs e)
        {

            string fileName = String.Empty;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\XML");
            saveFileDialog1.Filter = "XML files (*.xml)|*.XML";

            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFileDialog1.FileName;
            }
            else
                return;


            DataTable dt = new DataTable();
            for (int i = 1; i < dataGridView1.Columns.Count + 1; i++)
            {
                DataColumn column = new DataColumn(dataGridView1.Columns[i - 1].HeaderText);
                //string    
                dt.Columns.Add(column);
            }
            int columnCount = dataGridView1.Columns.Count;
            foreach (DataGridViewRow dr in dataGridView1.Rows)
            {
                DataRow dataRow = dt.NewRow();
                for (int i = 0; i < columnCount; i++)
                {
                    //returns checkboxes and dropdowns as string with .value..... nearly got it
                    dataRow[i] = dr.Cells[i].Value;
                }
                dt.Rows.Add(dataRow);
            }

            DataSet ds = new DataSet();
            ds.Tables.Add(dt);
            XmlTextWriter xmlSave = new XmlTextWriter(fileName, Encoding.UTF8);

            ds.WriteXml(xmlSave);
            xmlSave.Close();


            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------

        }

        private void button9_Click(object sender, EventArgs e)
        {

            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------



            DialogResult dialogResult = MessageBox.Show("By opening you will lost current results!", "WARNING!", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                //do something

                OpenFileDialog fdlg = new OpenFileDialog();
                fdlg.Title = "Select file";
                fdlg.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\XML");
                fdlg.FileName = txtFileName.Text;
                fdlg.Filter = "Excel Sheet(*.xml)|*.xml|All Files(*.*)|*.*";
                fdlg.FilterIndex = 1;
                fdlg.RestoreDirectory = true;
                if (fdlg.ShowDialog() == DialogResult.OK)
                {
                    dataGridView1.DataSource = null;
                    dataGridView1.Refresh();

                    dataGridView1.Columns.Add("", " ");
                    dataGridView1.Rows.Clear();
                    dataGridView1.Columns.Clear();
                    dataGridView1.Refresh();
                    txtFileName.Text = fdlg.FileName;












                    XmlReader xmlFile = XmlReader.Create(fdlg.FileName, new XmlReaderSettings());
                    DataSet dataSet = new DataSet();
                    dataSet.ReadXml(xmlFile);
                    //LOADS!!! YAY, but only the headers with no data... check save?
                    dataGridView1.DataSource = dataSet.Tables[0];
                    xmlFile.Close();

                    Application.DoEvents();
                    int tttt = dataGridView1.ColumnCount;
                    for (int y = 11; y < tttt; y++)
                    {
                        this.dataGridView1.Columns[y].Visible = false;
                    }
                    return;
                }

            }
            else if (dialogResult == DialogResult.No)
            {
                return;
            }
            return;


            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------

        }

        private void button7_Click(object sender, EventArgs e)
        {
            dataGridView2.DataSource = null;
            dataGridView2.Refresh();
            dataGridView2.Columns.Clear();

            dataGridView2.Columns.Add("", "UIDr");
            dataGridView2.Columns.Add("", "NAMEr");
            dataGridView2.Columns.Add("", "CATr");
            dataGridView2.Columns.Add("", "COACHr");
            dataGridView2.Columns.Add("", "COMMr");
            dataGridView2.Columns.Add("", "CLUBr");

            dataGridView2.Rows.Clear();

            dataGridView2.Refresh();
        }








        private void button5_Click(object sender, EventArgs e)
        {


            string fileName = String.Empty;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\EXCEL");
            saveFileDialog1.Filter = "XLS files (*.xls)|*.XLS";

            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFileDialog1.FileName;
            }
            else
                return;



            // creating Excel Application
            Microsoft.Office.Interop.Excel._Application app = new Microsoft.Office.Interop.Excel.Application();


            // creating new WorkBook within Excel application
            Microsoft.Office.Interop.Excel._Workbook workbook = app.Workbooks.Add(Type.Missing);


            // creating new Excelsheet in workbook
            Microsoft.Office.Interop.Excel._Worksheet worksheet = null;

            // see the excel sheet behind the program
            app.Visible = false;

            // get the reference of first sheet. By default its name is Sheet1.
            // store its reference to worksheet

            //            (Excel.Worksheet)xlWorkBook.Worksheets.get_Item(1)
            worksheet = workbook.Sheets.get_Item(1);
            worksheet = workbook.ActiveSheet;

            // changing the name of active sheet
            worksheet.Name = "Registration";


            // storing header part in Excel
            for (int i = 1; i < dataGridView2.Columns.Count + 1; i++)
            {
                worksheet.Cells[1, i] = dataGridView2.Columns[i - 1].HeaderText;
            }



            // storing Each row and column value to excel sheet
            for (int i = 0; i < dataGridView2.Rows.Count - 1; i++)
            {
                for (int j = 0; j < dataGridView2.Columns.Count; j++)
                {
                    if (dataGridView2.Rows[i].Cells[j].Value == null)
                    {
                        dataGridView2.Rows[i].Cells[j].Value = " ";
                    }
                    worksheet.Cells[i + 2, j + 1] = dataGridView2.Rows[i].Cells[j].Value.ToString();
                }
            }


            // save the application
            //workbook.SaveAs(fileName, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Microsoft.Office.Interop.Excel.XlSaveAsAccessMode.xlExclusive, Type.Missing, Type.Missing, Type.Missing, Type.Missing);
            workbook.SaveAs(fileName, Excel.XlFileFormat.xlWorkbookNormal, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Excel.XlSaveAsAccessMode.xlExclusive, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing);

            // Exit from the application
            app.Quit();


        }








        private void button6_Click(object sender, EventArgs e)
        {

            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------

            DialogResult dialogResult = MessageBox.Show("By opening you will lost current results!", "WARNING!", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                //do something

                OpenFileDialog fdlg = new OpenFileDialog();
                fdlg.Title = "Select file";
                fdlg.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\EXCEL");
                fdlg.FileName = txtFileName.Text;
                fdlg.Filter = "Excel Sheet(*.xls)|*.xls|All Files(*.*)|*.*";
                fdlg.FilterIndex = 1;
                fdlg.RestoreDirectory = true;
                if (fdlg.ShowDialog() == DialogResult.OK)
                {
                    dataGridView2.DataSource = null;
                    dataGridView2.Refresh();

                    dataGridView2.Columns.Add("", " ");
                    dataGridView2.Rows.Clear();
                    dataGridView2.Columns.Clear();
                    dataGridView2.Refresh();
                    txtFileName.Text = fdlg.FileName;
                    Import2();
                    Application.DoEvents();
                }


            }
            else if (dialogResult == DialogResult.No)
            {
                return;
            }
            return;
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
        }


        private void resyncregister()
        {
            int n = dataGridView2.Rows.Count;
            //  MessageBox.Show(n.ToString());

            int m = dataGridView1.Rows.Count;
            // MessageBox.Show(m.ToString());
            String suid;

            for (int j = 0; j < m; j++)
            {

                suid = dataGridView1.Rows[j].Cells[0].Value.ToString();
                foreach (DataGridViewRow row in dataGridView2.Rows)
                {
                    if (row.Cells[0].Value.ToString().Equals(suid))
                    {
                        dataGridView1.Rows[j].Cells[1].Value = row.Cells[1].Value;
                        dataGridView1.Rows[j].Cells[7].Value = row.Cells[2].Value;
                        dataGridView1.Rows[j].Cells[8].Value = row.Cells[3].Value;
                        dataGridView1.Rows[j].Cells[9].Value = row.Cells[4].Value;
                        dataGridView1.Rows[j].Cells[10].Value = row.Cells[5].Value;
                        break;
                    }


                }
            }
        }



        private void button10_Click(object sender, EventArgs e)
        {
            try { resyncregister(); }
            catch { return; }
        }

        private void ComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            
            
            ComboBox1.ForeColor = Color.Salmon;

            try
            {

                // MessageBox.Show((string)ComboBox1.SelectedValue);
                serialPort1.PortName = ComboBox1.Text;
                serialPort1.BaudRate = 115200;
                // MessageBox.Show(serialPort1.PortName.ToString ());
                serialPort1.Open();

                ComboBox1.ForeColor = Color.LightGreen;
            }
            catch { ComboBox1.SelectedIndex = 0; ComboBox1.ForeColor = Color.Salmon; MessageBox.Show("Select port before start!"); return; }
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            p1.BackColor = Color.DimGray;
            p2.BackColor = Color.DimGray;
            p3.BackColor = Color.DimGray;
            p4.BackColor = Color.DimGray;
            timer2.Stop();
        }

        private void button11_Click(object sender, EventArgs e)
        {

        }

        private void button12_Click(object sender, EventArgs e)
        {

        }

        private void button13_Click(object sender, EventArgs e)
        {


            string fileName = String.Empty;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\EXCEL");
            saveFileDialog1.Filter = "XLS files (*.xls)|*.XLS";

            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFileDialog1.FileName;
            }
            else
                return;



            // creating Excel Application
            Microsoft.Office.Interop.Excel._Application app = new Microsoft.Office.Interop.Excel.Application();


            // creating new WorkBook within Excel application
            Microsoft.Office.Interop.Excel._Workbook workbook = app.Workbooks.Add(Type.Missing);


            // creating new Excelsheet in workbook
            Microsoft.Office.Interop.Excel._Worksheet worksheet = null;

            // see the excel sheet behind the program
            app.Visible = false;

            // get the reference of first sheet. By default its name is Sheet1.
            // store its reference to worksheet

            //            (Excel.Worksheet)xlWorkBook.Worksheets.get_Item(1)
            worksheet = workbook.Sheets.get_Item(1);
            worksheet = workbook.ActiveSheet;

            // changing the name of active sheet
            worksheet.Name = "Results";


            // storing header part in Excel
            for (int i = 1; i < dataGridView1.Columns.Count + 1; i++)
            {
                worksheet.Cells[1, i] = dataGridView1.Columns[i - 1].HeaderText;
            }



            // storing Each row and column value to excel sheet
            for (int i = 0; i < dataGridView1.Rows.Count - 1; i++)
            {
                for (int j = 0; j < dataGridView1.Columns.Count; j++)
                {
                    if (dataGridView1.Rows[i].Cells[j].Value == null)
                    {
                        dataGridView1.Rows[i].Cells[j].Value = " ";
                    }
                    worksheet.Cells[i + 2, j + 1] = dataGridView1.Rows[i].Cells[j].Value.ToString();
                }
            }


            // save the application
            //workbook.SaveAs(fileName, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Microsoft.Office.Interop.Excel.XlSaveAsAccessMode.xlExclusive, Type.Missing, Type.Missing, Type.Missing, Type.Missing);
            workbook.SaveAs(fileName, Excel.XlFileFormat.xlWorkbookNormal, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Excel.XlSaveAsAccessMode.xlExclusive, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing);

            // Exit from the application
            app.Quit();
        }



        private void releaseObject(object obj)
        {

            try
            {

                System.Runtime.InteropServices.Marshal.ReleaseComObject(obj);

                obj = null;

            }

            catch (Exception ex)
            {

                obj = null;

                MessageBox.Show("Exception Occured while releasing object " + ex.ToString());

            }

            finally
            {

                GC.Collect();

            }

        }
        public static string SelectedTable = string.Empty;
        private void button14_Click(object sender, EventArgs e)
        {


            DialogResult dialogResult = MessageBox.Show("By opening you will lost current results!", "WARNING!", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                //do something

                OpenFileDialog fdlg = new OpenFileDialog();
                fdlg.Title = "Select file";
                fdlg.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\EXCEL");
                fdlg.FileName = txtFileName.Text;
                fdlg.Filter = "Excel Sheet(*.xls)|*.xls|All Files(*.*)|*.*";
                fdlg.FilterIndex = 1;
                fdlg.RestoreDirectory = true;
                if (fdlg.ShowDialog() == DialogResult.OK)
                {
                    dataGridView1.DataSource = null;
                    dataGridView1.Refresh();

                    dataGridView1.Columns.Add("", " ");
                    dataGridView1.Rows.Clear();
                    dataGridView1.Columns.Clear();
                    dataGridView1.Refresh();
                    txtFileName.Text = fdlg.FileName;
                    Import();
                    Application.DoEvents();
                }


                    int tttt = dataGridView1.ColumnCount;
                    for (int y = 11; y < tttt; y++)
                    {
                        this.dataGridView1.Columns[y].Visible = false;
                    }
                    return;

               
            }
            else if (dialogResult == DialogResult.No)
            {
                return;
            }
            return;
        }


        private DataGridView CopyDataGridView(DataGridView dgv_org)
        {
            DataGridView dgv_copy = new DataGridView();
            try
            {
                if (dgv_copy.Columns.Count == 0)
                {
                    foreach (DataGridViewColumn dgvc in dgv_org.Columns)
                    {
                        dgv_copy.Columns.Add(dgvc.Clone() as DataGridViewColumn);
                    }
                }

                DataGridViewRow row = new DataGridViewRow();

                for (int i = 0; i < dgv_org.Rows.Count; i++)
                {
                    row = (DataGridViewRow)dgv_org.Rows[i].Clone();
                    int intColIndex = 0;
                    foreach (DataGridViewCell cell in dgv_org.Rows[i].Cells)
                    {
                        row.Cells[intColIndex].Value = cell.Value;
                        intColIndex++;
                    }
                    dgv_copy.Rows.Add(row);
                }
                dgv_copy.AllowUserToAddRows = false;
                dgv_copy.Refresh();

            }
            catch { }
            return dgv_copy;
        }










        private void Import()
        {
            if (txtFileName.Text.Trim() != string.Empty)
            {
                try
                {
                    string[] strTables = GetTableExcel(txtFileName.Text);

                    frmSelectTables objSelectTable = new frmSelectTables(strTables);
                    objSelectTable.ShowDialog(this);
                    objSelectTable.Dispose();
                    if ((SelectedTable != string.Empty) && (SelectedTable != null))
                    {
                        DataTable dt = GetDataTableExcel(txtFileName.Text, SelectedTable);
                        dataGridView1.DataSource = dt.DefaultView;
                        dataGridView1.Refresh();

                    }
                }
                catch
                {
                    MessageBox.Show("Sorry");
                    dataGridView1.Refresh();
                }
            }

        }

        private void Import2()
        {
            if (txtFileName.Text.Trim() != string.Empty)
            {
                try
                {
                    string[] strTables = GetTableExcel(txtFileName.Text);

                    frmSelectTables objSelectTable = new frmSelectTables(strTables);
                    objSelectTable.ShowDialog(this);
                    objSelectTable.Dispose();
                    if ((SelectedTable != string.Empty) && (SelectedTable != null))
                    {
                        DataTable dt = GetDataTableExcel(txtFileName.Text, SelectedTable);
                        dataGridView2.DataSource = dt.DefaultView;
                        dataGridView2.Refresh();

                    }
                }
                catch
                {
                    MessageBox.Show("Sorry");
                    dataGridView2.Refresh();
                }
            }

        }

        public static DataTable GetDataTableExcel(string strFileName, string Table)
        {
            System.Data.OleDb.OleDbConnection conn = new System.Data.OleDb.OleDbConnection("Provider=Microsoft.Jet.OleDb.4.0; Data Source = " + strFileName + "; Extended Properties = \"Excel 8.0;HDR=Yes;IMEX=1\";");
            conn.Open();
            string strQuery = "SELECT * FROM [" + Table + "]";
            System.Data.OleDb.OleDbDataAdapter adapter = new System.Data.OleDb.OleDbDataAdapter(strQuery, conn);
            System.Data.DataSet ds = new System.Data.DataSet();
            adapter.Fill(ds);
            return ds.Tables[0];
        }

        public static string[] GetTableExcel(string strFileName)
        {
            string[] strTables = new string[100];
            Catalog oCatlog = new Catalog();
            ADOX.Table oTable = new ADOX.Table();
            ADODB.Connection oConn = new ADODB.Connection();
            oConn.Open("Provider=Microsoft.Jet.OleDb.4.0; Data Source = " + strFileName + "; Extended Properties = \"Excel 8.0;HDR=Yes;IMEX=1\";", "", "", 0);
            oCatlog.ActiveConnection = oConn;
            if (oCatlog.Tables.Count > 0)
            {
                int item = 0;
                foreach (ADOX.Table tab in oCatlog.Tables)
                {
                    if (tab.Type == "TABLE")
                    {
                        strTables[item] = tab.Name;
                        item++;
                    }
                }
            }
            return strTables;
        }

        private void customerOrderDataBindingSource_CurrentChanged(object sender, EventArgs e)
        {

        }

        private void button11_Click_1(object sender, EventArgs e)
        {
            dataGridView1.Rows.Add("$$$$$$$$");

        }

        private void button12_Click_1(object sender, EventArgs e)
        {
            dataGridView1.ClearSelection();
        }

        private void button15_Click(object sender, EventArgs e)
        {
            dataGridView1[0, 0].Selected = true;

        }
        int seqq = 0;
        private void button2_Click_1(object sender, EventArgs e)
        {
            if (seqq == 0)
            {
                seqq = 1;
                this.dataGridView1.Columns[6].Visible = false;
            }
            else if (seqq == 1)
            {
                seqq = 0;
                this.dataGridView1.Columns[6].Visible = true;
            }
        }

        private void button16_Click(object sender, EventArgs e)
        {

            string fileName = String.Empty;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\XML");
            saveFileDialog1.Filter = "XML files (*.xml)|*.XML";

            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFileDialog1.FileName;
            }
            else
                return;


            DataTable dt2 = new DataTable();
            for (int i = 1; i < dataGridView2.Columns.Count + 1; i++)
            {
                DataColumn column = new DataColumn(dataGridView2.Columns[i - 1].HeaderText);
                //string    
                dt2.Columns.Add(column);
            }
            int columnCount = dataGridView2.Columns.Count;
            foreach (DataGridViewRow dr2 in dataGridView2.Rows)
            {
                DataRow dataRow2 = dt2.NewRow();
                for (int i = 0; i < columnCount; i++)
                {
                    //returns checkboxes and dropdowns as string with .value..... nearly got it
                    dataRow2[i] = dr2.Cells[i].Value;
                }
                dt2.Rows.Add(dataRow2);
            }

            DataSet ds2 = new DataSet();
            ds2.Tables.Add(dt2);
            XmlTextWriter xmlSave = new XmlTextWriter(fileName, Encoding.UTF8);

            ds2.WriteXml(xmlSave);
            xmlSave.Close();


            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------

        }

        private void button11_Click_2(object sender, EventArgs e)
        {
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------



            DialogResult dialogResult = MessageBox.Show("By opening you will lost current results!", "WARNING!", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                //do something

                OpenFileDialog fdlg = new OpenFileDialog();
                fdlg.Title = "Select file";
                fdlg.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\XML");
                fdlg.FileName = txtFileName.Text;
                fdlg.Filter = "Excel Sheet(*.xml)|*.xml|All Files(*.*)|*.*";
                fdlg.FilterIndex = 1;
                fdlg.RestoreDirectory = true;
                if (fdlg.ShowDialog() == DialogResult.OK)
                {
                    dataGridView2.DataSource = null;
                    dataGridView2.Refresh();

                    dataGridView2.Columns.Add("", " ");
                    dataGridView2.Rows.Clear();
                    dataGridView2.Columns.Clear();
                    dataGridView2.Refresh();
                    txtFileName.Text = fdlg.FileName;












                    XmlReader xmlFile = XmlReader.Create(fdlg.FileName, new XmlReaderSettings());
                    DataSet dataSet = new DataSet();
                    dataSet.ReadXml(xmlFile);
                    //LOADS!!! YAY, but only the headers with no data... check save?
                    dataGridView2.DataSource = dataSet.Tables[0];
                    xmlFile.Close();

















                    Application.DoEvents();
                }

            }
            else if (dialogResult == DialogResult.No)
            {
                return;
            }
            return;


            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {
            dataGridView1.ClearSelection();
            string search = textBox3.Text;

            try
            {
                //                row.Selected = true;

                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    foreach (DataGridViewCell cell in row.Cells)
                    {
                        if (cell.Value == DBNull.Value || cell.Value == null)
                            continue;
                        if (cell.Value.ToString().Contains(search))
                        {
                            cell.Selected = true;
                        }
                    }
                }


            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message);
            }

            //---------------------------------------------------------------------------
        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {
            dataGridView2.ClearSelection();
            string search = textBox4.Text;

            try
            {
                //                row.Selected = true;

                foreach (DataGridViewRow row in dataGridView2.Rows)
                {
                    foreach (DataGridViewCell cell in row.Cells)
                    {
                        if (cell.Value == DBNull.Value || cell.Value == null)
                            continue;
                        if (cell.Value.ToString().Contains(search))
                        {
                            cell.Selected = true;
                        }
                    }
                }


            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message);
            }

        }

        private void p4_MouseClick(object sender, MouseEventArgs e)
        {
            if (k == 0)
            {
                panel3.Visible = true;
                k = k + 1;
                dataGridView1.Size = new Size(1005, 243);
            }
            else if (k == 1)
            {
                //panel3.Visible = false;
                k = k + 1;
                panel3.Size = new Size(1005, 582);
                panel3.Location = new Point(10, 9);
                dataGridView2.Size = new Size(1004, 549);


            }
            else if (k == 2)
            {
                panel3.Size = new Size(1006, 310);
                panel3.Location = new Point(10, 280);
                panel3.Visible = false;
                k = 0;
                dataGridView2.Size = new Size(1004, 278);
                dataGridView1.Size = new Size(1005, 554);
            }
        }

        private void p4_MouseMove(object sender, MouseEventArgs e)
        {
            label1.Visible = true;
            regb.Visible = true;
            timerb.Visible = true;
        
        }

        private void p4_MouseLeave(object sender, EventArgs e)
        {
            
        }

        private void p3_MouseClick(object sender, MouseEventArgs e)
        {


        }
       
        private void c1_Click(object sender, EventArgs e)
        {
            go = 0;
            c1.BackColor = Color.LightGreen;
            c2.BackColor = Color.Salmon;
        }

        private void button12_Click_2(object sender, EventArgs e)
        {
            go = 1;
            c2.BackColor = Color.LightGreen;
            c1.BackColor = Color.Salmon;
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------



            DialogResult dialogResult = MessageBox.Show("By opening you will lost current results!", "WARNING!", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                //do something

                OpenFileDialog fdlg = new OpenFileDialog();
                fdlg.Title = "Select file";
                fdlg.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\XML");
                fdlg.FileName = txtFileName.Text;
                fdlg.Filter = "Excel Sheet(*.xml)|*.xml|All Files(*.*)|*.*";
                fdlg.FilterIndex = 1;
                fdlg.RestoreDirectory = true;
                if (fdlg.ShowDialog() == DialogResult.OK)
                {
                    dataGridView1.DataSource = null;
                    dataGridView1.Refresh();

                    dataGridView1.Columns.Add("", " ");
                    dataGridView1.Rows.Clear();
                    dataGridView1.Columns.Clear();
                    dataGridView1.Refresh();
                    txtFileName.Text = fdlg.FileName;












                    XmlReader xmlFile = XmlReader.Create(fdlg.FileName, new XmlReaderSettings());
                    DataSet dataSet = new DataSet();
                    dataSet.ReadXml(xmlFile);
                    //LOADS!!! YAY, but only the headers with no data... check save?
                    dataGridView1.DataSource = dataSet.Tables[0];
                   
                    filterDataGridView1.DataSource = dataSet.Tables[0];
                    xmlFile.Close();

                    Application.DoEvents();
                    int tttt = dataGridView1.ColumnCount;
                    for (int y = 11; y < tttt; y++)
                    {
                        this.dataGridView1.Columns[y].Visible = false;
                    }
                    return;
                }

            }
            else if (dialogResult == DialogResult.No)
            {
                return;
            }
            return;


            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------

        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {

            string fileName = String.Empty;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\XML");
            saveFileDialog1.Filter = "XML files (*.xml)|*.XML";

            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFileDialog1.FileName;
            }
            else
                return;


            DataTable dt = new DataTable();
            for (int i = 1; i < dataGridView1.Columns.Count + 1; i++)
            {
                DataColumn column = new DataColumn(dataGridView1.Columns[i - 1].HeaderText);
                //string    
                dt.Columns.Add(column);
            }
            int columnCount = dataGridView1.Columns.Count;
            foreach (DataGridViewRow dr in dataGridView1.Rows)
            {
                DataRow dataRow = dt.NewRow();
                for (int i = 0; i < columnCount; i++)
                {
                    //returns checkboxes and dropdowns as string with .value..... nearly got it
                    dataRow[i] = dr.Cells[i].Value;
                }
                dt.Rows.Add(dataRow);
            }

            DataSet ds = new DataSet();
            ds.Tables.Add(dt);
            XmlTextWriter xmlSave = new XmlTextWriter(fileName, Encoding.UTF8);

            ds.WriteXml(xmlSave);
            xmlSave.Close();


            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {


            DialogResult dialogResult = MessageBox.Show("By opening you will lost current results!", "WARNING!", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                //do something

                OpenFileDialog fdlg = new OpenFileDialog();
                fdlg.Title = "Select file";
                fdlg.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\EXCEL");
                fdlg.FileName = txtFileName.Text;
                fdlg.Filter = "Excel Sheet(*.xls)|*.xls|All Files(*.*)|*.*";
                fdlg.FilterIndex = 1;
                fdlg.RestoreDirectory = true;
                if (fdlg.ShowDialog() == DialogResult.OK)
                {
                    dataGridView1.DataSource = null;
                    dataGridView1.Refresh();

                    dataGridView1.Columns.Add("", " ");
                    dataGridView1.Rows.Clear();
                    dataGridView1.Columns.Clear();
                    dataGridView1.Refresh();
                    txtFileName.Text = fdlg.FileName;
                    Import();
                    Application.DoEvents();
                }


                int tttt = dataGridView1.ColumnCount;
                for (int y = 11; y < tttt; y++)
                {
                    this.dataGridView1.Columns[y].Visible = false;
                }
                return;


            }
            else if (dialogResult == DialogResult.No)
            {
                return;
            }
            return;
        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {


            string fileName = String.Empty;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\EXCEL");
            saveFileDialog1.Filter = "XLS files (*.xls)|*.XLS";

            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFileDialog1.FileName;
            }
            else
                return;



            // creating Excel Application
            Microsoft.Office.Interop.Excel._Application app = new Microsoft.Office.Interop.Excel.Application();


            // creating new WorkBook within Excel application
            Microsoft.Office.Interop.Excel._Workbook workbook = app.Workbooks.Add(Type.Missing);


            // creating new Excelsheet in workbook
            Microsoft.Office.Interop.Excel._Worksheet worksheet = null;

            // see the excel sheet behind the program
            app.Visible = false;

            // get the reference of first sheet. By default its name is Sheet1.
            // store its reference to worksheet

            //            (Excel.Worksheet)xlWorkBook.Worksheets.get_Item(1)
            worksheet = workbook.Sheets.get_Item(1);
            worksheet = workbook.ActiveSheet;

            // changing the name of active sheet
            worksheet.Name = "Results";


            // storing header part in Excel
            for (int i = 1; i < dataGridView1.Columns.Count + 1; i++)
            {
                worksheet.Cells[1, i] = dataGridView1.Columns[i - 1].HeaderText;
            }



            // storing Each row and column value to excel sheet
            for (int i = 0; i < dataGridView1.Rows.Count - 1; i++)
            {
                for (int j = 0; j < dataGridView1.Columns.Count; j++)
                {
                    if (dataGridView1.Rows[i].Cells[j].Value == null)
                    {
                        dataGridView1.Rows[i].Cells[j].Value = " ";
                    }
                    worksheet.Cells[i + 2, j + 1] = dataGridView1.Rows[i].Cells[j].Value.ToString();
                }
            }


            // save the application
            //workbook.SaveAs(fileName, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Microsoft.Office.Interop.Excel.XlSaveAsAccessMode.xlExclusive, Type.Missing, Type.Missing, Type.Missing, Type.Missing);
            workbook.SaveAs(fileName, Excel.XlFileFormat.xlWorkbookNormal, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Excel.XlSaveAsAccessMode.xlExclusive, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing);

            // Exit from the application
            app.Quit();
        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {
            int t = dataGridView1.ColumnCount;
            if (hl == 0)
            {
                hl = 1;
                for (int y = 11; y < t; y++)
                {
                    this.dataGridView1.Columns[y].Visible = false;
                }

            }
            else if (hl == 1)
            {
                hl = 0;
                for (int y = 11; y < t; y++)
                {
                    this.dataGridView1.Columns[y].Visible = true;
                }
            }
        }

        private void pictureBox6_Click(object sender, EventArgs e)
        {
            if (seqq == 0)
            {
                seqq = 1;
                this.dataGridView1.Columns[6].Visible = false;
            }
            else if (seqq == 1)
            {
                seqq = 0;
                this.dataGridView1.Columns[6].Visible = true;
            }
        }

        private void pictureBox7_Click(object sender, EventArgs e)
        {
            dataGridView1.DataSource = null;
            dataGridView1.Refresh();
            dataGridView1.Columns.Clear();

            dataGridView1.Columns.Add("", "UID");
            dataGridView1.Columns.Add("", "NAME");
            dataGridView1.Columns.Add("", "RT(ms)");
            dataGridView1.Columns.Add("", "RT");
            dataGridView1.Columns.Add("", "START");
            dataGridView1.Columns.Add("", "FINISH");
            dataGridView1.Columns.Add("", "SEQUENCE");
            dataGridView1.Columns.Add("", "CAT");
            dataGridView1.Columns.Add("", "COACH");
            dataGridView1.Columns.Add("", "COMM");
            dataGridView1.Columns.Add("", "CLUB");
            dataGridView1.Rows.Clear();

            dataGridView1.Refresh();
        }

        private void pictureBox8_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "Filter";
        }

        private void pictureBox7_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "Clear all";
        }

        private void pictureBox6_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "Show/Hide Sequence";    
        }

        private void pictureBox5_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "Show/Hide Laps";  
        }

        private void pictureBox4_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "Save to Excel";  
        }

        private void pictureBox3_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "Read from Excel";  
        }

        private void pictureBox2_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "Save to XML";  
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "Read from XML";  
        }

        private void textBox3_MouseMove(object sender, MouseEventArgs e)
        {
            textBox3.Text = "";   
        }

        private void pb1_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "";  
        }

        private void dataGridView1_MouseMove(object sender, MouseEventArgs e)
        {
            label1.Visible = false;
            lh2.Text = "";
            lh1.Text = "";
            regb.Visible = false;
            timerb.Visible = false;
        }

        private void compet_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "";  
        }

        private void panel5_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "";  
        }

        private void pictureBox9_MouseMove(object sender, MouseEventArgs e)
        {
            lh1.Text = "Search";  
        }

        private void dataGridView2_MouseMove(object sender, MouseEventArgs e)
        {
            label1.Visible = false;
            lh2.Text = "";
            timerb.Visible = false;
            regb.Visible = false;
        }

        private void timerb_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("Timer.exe");
        }

        private void pictureBox13_Click(object sender, EventArgs e)
        {
            dataGridView2.DataSource = null;
            dataGridView2.Refresh();
            dataGridView2.Columns.Clear();

            dataGridView2.Columns.Add("", "UIDr");
            dataGridView2.Columns.Add("", "NAMEr");
            dataGridView2.Columns.Add("", "CATr");
            dataGridView2.Columns.Add("", "COACHr");
            dataGridView2.Columns.Add("", "COMMr");
            dataGridView2.Columns.Add("", "CLUBr");

            dataGridView2.Rows.Clear();

            dataGridView2.Refresh();
        }

        private void pictureBox14_Click(object sender, EventArgs e)
        {


            string fileName = String.Empty;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\EXCEL");
            saveFileDialog1.Filter = "XLS files (*.xls)|*.XLS";

            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFileDialog1.FileName;
            }
            else
                return;



            // creating Excel Application
            Microsoft.Office.Interop.Excel._Application app = new Microsoft.Office.Interop.Excel.Application();


            // creating new WorkBook within Excel application
            Microsoft.Office.Interop.Excel._Workbook workbook = app.Workbooks.Add(Type.Missing);


            // creating new Excelsheet in workbook
            Microsoft.Office.Interop.Excel._Worksheet worksheet = null;

            // see the excel sheet behind the program
            app.Visible = false;

            // get the reference of first sheet. By default its name is Sheet1.
            // store its reference to worksheet

            //            (Excel.Worksheet)xlWorkBook.Worksheets.get_Item(1)
            worksheet = workbook.Sheets.get_Item(1);
            worksheet = workbook.ActiveSheet;

            // changing the name of active sheet
            worksheet.Name = "Registration";


            // storing header part in Excel
            for (int i = 1; i < dataGridView2.Columns.Count + 1; i++)
            {
                worksheet.Cells[1, i] = dataGridView2.Columns[i - 1].HeaderText;
            }



            // storing Each row and column value to excel sheet
            for (int i = 0; i < dataGridView2.Rows.Count - 1; i++)
            {
                for (int j = 0; j < dataGridView2.Columns.Count; j++)
                {
                    if (dataGridView2.Rows[i].Cells[j].Value == null)
                    {
                        dataGridView2.Rows[i].Cells[j].Value = " ";
                    }
                    worksheet.Cells[i + 2, j + 1] = dataGridView2.Rows[i].Cells[j].Value.ToString();
                }
            }


            // save the application
            //workbook.SaveAs(fileName, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Microsoft.Office.Interop.Excel.XlSaveAsAccessMode.xlExclusive, Type.Missing, Type.Missing, Type.Missing, Type.Missing);
            workbook.SaveAs(fileName, Excel.XlFileFormat.xlWorkbookNormal, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Excel.XlSaveAsAccessMode.xlExclusive, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing);

            // Exit from the application
            app.Quit();
        }

        private void pictureBox15_Click(object sender, EventArgs e)
        {
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------

            DialogResult dialogResult = MessageBox.Show("By opening you will lost current results!", "WARNING!", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                //do something

                OpenFileDialog fdlg = new OpenFileDialog();
                fdlg.Title = "Select file";
                fdlg.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\EXCEL");
                fdlg.FileName = txtFileName.Text;
                fdlg.Filter = "Excel Sheet(*.xls)|*.xls|All Files(*.*)|*.*";
                fdlg.FilterIndex = 1;
                fdlg.RestoreDirectory = true;
                if (fdlg.ShowDialog() == DialogResult.OK)
                {
                    dataGridView2.DataSource = null;
                    dataGridView2.Refresh();

                    dataGridView2.Columns.Add("", " ");
                    dataGridView2.Rows.Clear();
                    dataGridView2.Columns.Clear();
                    dataGridView2.Refresh();
                    txtFileName.Text = fdlg.FileName;
                    Import2();
                    Application.DoEvents();
                }


            }
            else if (dialogResult == DialogResult.No)
            {
                return;
            }
            return;
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
        }

        private void pictureBox16_Click(object sender, EventArgs e)
        {

            string fileName = String.Empty;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\XML");
            saveFileDialog1.Filter = "XML files (*.xml)|*.XML";

            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFileDialog1.FileName;
            }
            else
                return;


            DataTable dt2 = new DataTable();
            for (int i = 1; i < dataGridView2.Columns.Count + 1; i++)
            {
                DataColumn column = new DataColumn(dataGridView2.Columns[i - 1].HeaderText);
                //string    
                dt2.Columns.Add(column);
            }
            int columnCount = dataGridView2.Columns.Count;
            foreach (DataGridViewRow dr2 in dataGridView2.Rows)
            {
                DataRow dataRow2 = dt2.NewRow();
                for (int i = 0; i < columnCount; i++)
                {
                    //returns checkboxes and dropdowns as string with .value..... nearly got it
                    dataRow2[i] = dr2.Cells[i].Value;
                }
                dt2.Rows.Add(dataRow2);
            }

            DataSet ds2 = new DataSet();
            ds2.Tables.Add(dt2);
            XmlTextWriter xmlSave = new XmlTextWriter(fileName, Encoding.UTF8);

            ds2.WriteXml(xmlSave);
            xmlSave.Close();


            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------

        }

        private void pictureBox17_Click(object sender, EventArgs e)
        {
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------



            DialogResult dialogResult = MessageBox.Show("By opening you will lost current results!", "WARNING!", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                //do something

                OpenFileDialog fdlg = new OpenFileDialog();
                fdlg.Title = "Select file";
                fdlg.InitialDirectory = Path.Combine(Application.StartupPath, @"Data\XML");
                fdlg.FileName = txtFileName.Text;
                fdlg.Filter = "Excel Sheet(*.xml)|*.xml|All Files(*.*)|*.*";
                fdlg.FilterIndex = 1;
                fdlg.RestoreDirectory = true;
                if (fdlg.ShowDialog() == DialogResult.OK)
                {
                    dataGridView2.DataSource = null;
                    dataGridView2.Refresh();

                    dataGridView2.Columns.Add("", " ");
                    dataGridView2.Rows.Clear();
                    dataGridView2.Columns.Clear();
                    dataGridView2.Refresh();
                    txtFileName.Text = fdlg.FileName;












                    XmlReader xmlFile = XmlReader.Create(fdlg.FileName, new XmlReaderSettings());
                    DataSet dataSet = new DataSet();
                    dataSet.ReadXml(xmlFile);
                    //LOADS!!! YAY, but only the headers with no data... check save?
                    dataGridView2.DataSource = dataSet.Tables[0];
                    xmlFile.Close();

















                    Application.DoEvents();
                }

            }
            else if (dialogResult == DialogResult.No)
            {
                return;
            }
            return;


            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------------

        }

        private void pictureBox18_Click(object sender, EventArgs e)
        {
            try { resyncregister(); }
            catch { return; }
        }

        private void pictureBox12_MouseMove(object sender, MouseEventArgs e)
        {
            lh2.Text = "Filter";
        }

        private void pictureBox13_MouseMove(object sender, MouseEventArgs e)
        {
            lh2.Text = "Clear all";
        }

        private void pictureBox18_MouseMove(object sender, MouseEventArgs e)
        {
            lh2.Text = "Resync";  
        }

        private void pictureBox14_MouseMove(object sender, MouseEventArgs e)
        {
            lh2.Text = "Save to Excel";  
        }

        private void pictureBox15_MouseMove(object sender, MouseEventArgs e)
        {
            lh2.Text = "Read from Excel"; 
        }

        private void pictureBox16_MouseMove(object sender, MouseEventArgs e)
        {
            lh2.Text = "Save to XML";  
        }

        private void pictureBox17_MouseMove(object sender, MouseEventArgs e)
        {
            lh2.Text = "Read from XML";
        }

        private void pictureBox11_MouseMove(object sender, MouseEventArgs e)
        {
            lh2.Text = "Search"; 
        }

        private void textBox4_MouseMove(object sender, MouseEventArgs e)
        {
            textBox4.Text = "";    
        }

        private void pictureBox10_MouseMove(object sender, MouseEventArgs e)
        {
            lh2.Text = "";
        }

        private void panel3_MouseMove(object sender, MouseEventArgs e)
        {
            lh2.Text = "";
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            label1.Text = DateTime.Now.ToLongTimeString();
        }
    }
}





        




        
