using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SimpleSerial
{
    public partial class menu : Form
    {
        public menu()
        {
            InitializeComponent();
        }

        private void menu_Load(object sender, EventArgs e)
        {

        }

        private void menu_Activated(object sender, EventArgs e)
        {
            label1.Hide();
            label2.Hide();
            label3.Hide();
            label4.Hide();
            panel6.Hide();
            panel7.Hide();
        }

        private void menu_MouseMove(object sender, MouseEventArgs e)
        {
            label1.Hide();
            label2.Hide();
            label3.Hide();
            label4.Hide();
            panel6.Hide();
            panel7.Hide();
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            this.Hide(); 
            compet a = new compet();

            a.ShowDialog();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox2_MouseMove(object sender, MouseEventArgs e)
        {
            label1.Show();
            label2.Hide();
            label3.Show();
            label4.Hide();
            panel6.Show();
            panel7.Hide();
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            label2.Show();
            label1.Hide();
            label4.Show();
            label3.Hide();
            panel6.Hide();
            panel7.Show();
        }

        private void menu_Load_1(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
