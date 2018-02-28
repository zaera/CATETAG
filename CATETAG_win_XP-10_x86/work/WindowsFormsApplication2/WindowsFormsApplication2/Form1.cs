using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using DataGridViewAutoFilter;
using System.Xml;
using System.IO;
namespace WindowsFormsApplication2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();







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
                //fdlg.FileName = txtFileName.Text;
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
                //    txtFileName.Text = fdlg.FileName;












                    XmlReader xmlFile = XmlReader.Create(fdlg.FileName, new XmlReaderSettings());
                    DataSet dataSet = new DataSet();
                    dataSet.ReadXml(xmlFile);
                    //LOADS!!! YAY, but only the headers with no data... check save?
                    dataGridView1.DataSource = dataSet.Tables[0];

                  //  filterDataGridView1.DataSource = dataSet.Tables[0];
                    xmlFile.Close();

                    Application.DoEvents();
                  
                }

            }
            else if (dialogResult == DialogResult.No)
            {
                return;
            }
//            return;



            /*
            // create sequence 
            Item[] items = new Item[] { new Book{Id = 1, Price = 13.50, Genre = "Comedy", Author = "Jim Bob"}, 
                                        new Book{Id = 2, Price = 8.50, Genre = "Drama", Author = "John Fox"},  
                                        new Movie{Id = 1, Price = 22.99, Genre = "Comedy", Director = "Phil Funk"},
                                        new Movie{Id = 1, Price = 13.40, Genre = "Action", Director = "Eddie Jones"}};

            */



            DataTable table = new DataTable();
            //dataGridView1.DataSource = table;
            for (int i = 0; i < dataGridView1.ColumnCount; ++i)
            {
  table.Columns.Add(new DataColumn(dataGridView1.Columns[i].Name));
  //dataGridView1.Columns[i].DataPropertyName = dataGridView1.Columns[i].Name;
}


            BindingSource source = new BindingSource();
            //source.DataSource = dataGridView1.DataSource;
            source.DataSource = table;
       

            foreach (DataColumn col in table.Columns)
            {
                DataGridViewAutoFilterTextBoxColumn commonColumn = new DataGridViewAutoFilterTextBoxColumn();
                commonColumn.DataPropertyName = col.ColumnName;
                commonColumn.HeaderText = col.ColumnName;
                commonColumn.Resizable = DataGridViewTriState.True;
                this.dataGridView1.Columns.Add(commonColumn);
            }            

            this.dataGridView1.DataSource = source;
        }

        private void dataGridView1_SelectionChanged(object sender, EventArgs e)
        {
            int iCount = this.dataGridView1.SelectedRows.Count;
            this.toolStripStatus_SelectedRows.Text = string.Format("{0} row{1} selected", iCount.ToString(), iCount > 1 ? "s" : "");
        }

        private void dataGridView1_DataBindingComplete(object sender, DataGridViewBindingCompleteEventArgs e)
        {
            BindingSource data = this.dataGridView1.DataSource as BindingSource;
            if (data == null || data.DataSource == null)
            {
                return;
            }

            /* Show total records number*/
            // Retrieve the unfiltered row count by 
            // temporarily unfiltering the data.
            data.RaiseListChangedEvents = false;
            String oldFilter = data.Filter;
            data.Filter = null;
            int iTotalNum = data.Count;
            this.toolStripStatus_Total.Text = string.Format("Total of {0} record{1}.", iTotalNum.ToString(), iTotalNum > 1 ? "s" : "");
            data.Filter = oldFilter;
            data.RaiseListChangedEvents = true;

            /* Show filter information.*/
            int iFilterNum = data.Count;
            string filter = data.Filter;
            if (String.IsNullOrEmpty(filter))
            {
                this.toolStripStatus_Separator2.Visible = false;
                this.toolStripStatus_Filter.Visible = false;
                this.toolStripStatus_ShowAll.Visible = false;
            }
            else
            {
                this.toolStripStatus_Separator2.Visible = true;
                this.toolStripStatus_Filter.Visible = true;
                this.toolStripStatus_ShowAll.Visible = true;
                this.toolStripStatus_Filter.Text = string.Format("{0} record{1} found.", iFilterNum.ToString(), iFilterNum > 1 ? "s" : "");
                this.toolStripStatus_Filter.Text += " (Filter: " + filter + ")";
            }
        }

        private void toolStripStatus_ShowAll_Click(object sender, EventArgs e)
        {
            DataGridViewAutoFilterColumnHeaderCell.RemoveFilter(this.dataGridView1);
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }
    }

    public class Item
    {
        public int Id { get; set; }
        public double Price { get; set; }
        public string Genre { get; set; }
    }

    public class Book : Item
    {
        public string Author { get; set; }
    }

    public class Movie : Item
    {
        public string Director { get; set; }
    }

    public static class DataSetLinqOperators
    {
        public static DataTable CopyToDataTable<T>(this IEnumerable<T> source)
        {
            return new ObjectShredder<T>().Shred(source, null, null);
        }

        public static DataTable CopyToDataTable<T>(this IEnumerable<T> source,
                                                    DataTable table, LoadOption? options)
        {
            return new ObjectShredder<T>().Shred(source, table, options);
        }

    }

    public class ObjectShredder<T>
    {
        private FieldInfo[] _fi;
        private PropertyInfo[] _pi;
        private Dictionary<string, int> _ordinalMap;
        private Type _type;

        public ObjectShredder()
        {
            _type = typeof(T);
            _fi = _type.GetFields();
            _pi = _type.GetProperties();
            _ordinalMap = new Dictionary<string, int>();
        }

        public DataTable Shred(IEnumerable<T> source, DataTable table, LoadOption? options)
        {
            if (typeof(T).IsPrimitive)
            {
                return ShredPrimitive(source, table, options);
            }


            if (table == null)
            {
                table = new DataTable(typeof(T).Name);
            }

            // now see if need to extend datatable base on the type T + build ordinal map
            table = ExtendTable(table, typeof(T));

            table.BeginLoadData();
            using (IEnumerator<T> e = source.GetEnumerator())
            {
                while (e.MoveNext())
                {
                    if (options != null)
                    {
                        table.LoadDataRow(ShredObject(table, e.Current), (LoadOption)options);
                    }
                    else
                    {
                        table.LoadDataRow(ShredObject(table, e.Current), true);
                    }
                }
            }
            table.EndLoadData();
            return table;
        }

        public DataTable ShredPrimitive(IEnumerable<T> source, DataTable table, LoadOption? options)
        {
            if (table == null)
            {
                table = new DataTable(typeof(T).Name);
            }

            if (!table.Columns.Contains("Value"))
            {
                table.Columns.Add("Value", typeof(T));
            }

            table.BeginLoadData();
            using (IEnumerator<T> e = source.GetEnumerator())
            {
                Object[] values = new object[table.Columns.Count];
                while (e.MoveNext())
                {
                    values[table.Columns["Value"].Ordinal] = e.Current;

                    if (options != null)
                    {
                        table.LoadDataRow(values, (LoadOption)options);
                    }
                    else
                    {
                        table.LoadDataRow(values, true);
                    }
                }
            }
            table.EndLoadData();
            return table;
        }

        public DataTable ExtendTable(DataTable table, Type type)
        {
            // value is type derived from T, may need to extend table.
            foreach (FieldInfo f in type.GetFields())
            {
                if (!_ordinalMap.ContainsKey(f.Name))
                {
                    DataColumn dc = table.Columns.Contains(f.Name) ? table.Columns[f.Name]
                        : table.Columns.Add(f.Name, f.FieldType);
                    _ordinalMap.Add(f.Name, dc.Ordinal);
                }
            }
            foreach (PropertyInfo p in type.GetProperties())
            {
                if (!_ordinalMap.ContainsKey(p.Name))
                {
                    DataColumn dc = table.Columns.Contains(p.Name) ? table.Columns[p.Name]
                        : table.Columns.Add(p.Name, p.PropertyType);
                    _ordinalMap.Add(p.Name, dc.Ordinal);
                }
            }
            return table;
        }

        public object[] ShredObject(DataTable table, T instance)
        {

            FieldInfo[] fi = _fi;
            PropertyInfo[] pi = _pi;

            if (instance.GetType() != typeof(T))
            {
                ExtendTable(table, instance.GetType());
                fi = instance.GetType().GetFields();
                pi = instance.GetType().GetProperties();
            }

            Object[] values = new object[table.Columns.Count];
            foreach (FieldInfo f in fi)
            {
                values[_ordinalMap[f.Name]] = f.GetValue(instance);
            }

            foreach (PropertyInfo p in pi)
            {
                values[_ordinalMap[p.Name]] = p.GetValue(instance, null);
            }
            return values;
        }
    }
}
