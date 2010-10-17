using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AVRConnector;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        AVRConnection c = new AVRConnection();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            c.ping();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            c.set_led(0);
        }
        
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            c.set_led(checkBox1.Checked ? (byte)1 : (byte)0);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            textBox1.Text = c.get_temp_vorlauf().ToString("N2");
        }
    }
}
