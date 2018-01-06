#pragma warning disable
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;


namespace Client1
{
    /// <summary>
    /// </summary>
    public partial class MainWindow : Window
    {
        static string downLoadFolder = System.IO.Path.GetFullPath(@"..\..\clnt1downloads");
        Shim shim = new Shim(8080, downLoadFolder);
        Thread tRcv = null;
        public MainWindow()
        {
            InitializeComponent();
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Console.WriteLine("Window loaded");
            tRcv = new Thread(
              () =>
              {
                  while (true)
                  {
                      String msg = shim.GetMessage();
                      Action<String> act = new Action<string>(processMsgRecievedByClient);
                      Object[] args = { msg };
                      Dispatcher.Invoke(act, args);
                  }
              }
            );
            tRcv.Start();
        }
        private void processMsgRecievedByClient(String msg)
        {
            String msgType = "";
            string msgBody = "";
            if (msg.Contains(";;"))
            {
                msg.Substring(msg.IndexOf(";;") + 2, 4);

                msg.Substring(0, msg.IndexOf(";;"));
            }

            switch (msgType)
            {
                case "text":
                    processIncomingTextMessage(msgBody);
                    break;
                default:
                    processIncomingTextMessage(msg);
                    break;
            }


        }

       
        private void processIncomingTextMessage(string msgBody)
        {
            int command = int.Parse(findMsgAttribute(msgBody, "command"));
            switch (command)
            {
                case 5:
                    reenableAnalysis();
                    break;
                case 6:
                    updateCategoryList(msgBody);
                    break;
                case 7:
                    updateserverFileListForCategoty(msgBody);
                    break;

                default:
                    break;
            }

        }

        private void updateserverFileListForCategoty(string msgBody)
        {
            string filesSerialised = findMsgAttribute(msgBody, "fNames");
            List<string> files = filesSerialised.Split(':').ToList();
            lstServerFiles.Items.Clear();
            foreach (var file in files)
            {
                if (file.Length>0)
                {
                    lstServerFiles.Items.Add(file);
                }
            }

        }

        private void updateCategoryList(string msgBody)
        {
            string catgString = findMsgAttribute(msgBody, "catg");
            List<string> catgs = catgString.Split(':').ToList();
            cbCategories.Items.Clear();
            foreach (var catg in catgs)
            {
                if (catg.Length > 0)
                {
                    cbCategories.Items.Add(catg);
                }
            }
            cbCategories.SelectedIndex = 0;
        }

        private void reenableAnalysis()
        {
            btnAnalyze.Content = "Publish";
            btnAnalyze.IsEnabled = true;
            btnSendFiles.Content = "Send File";
            btnSendFiles.IsEnabled = true;

        }

        private string findMsgAttribute(string msgBody, string attribute)
        {
            int sI = (msgBody.IndexOf("<" + attribute + ">") + (attribute.Length + 2));
            int eI = msgBody.IndexOf("</" + attribute + ">");
            return msgBody.Substring(sI, eI - sI);
        }
        
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            
            Microsoft.Win32.OpenFileDialog openFileDialog = new Microsoft.Win32.OpenFileDialog();
            openFileDialog.Filter = "cpp/h Files (*.cpp;*.h)|*.cpp;*.h";
            openFileDialog.Multiselect = true;
            openFileDialog.InitialDirectory = System.IO.Path.GetFullPath(@"..\..\CodeTobeAnalyzed");
            Console.WriteLine(Directory.GetCurrentDirectory());
            Console.WriteLine(Environment.CurrentDirectory);
            if (openFileDialog.ShowDialog() == true)
            {
                lstSendFiles.SelectionMode = SelectionMode.Multiple;
                foreach (string filename in openFileDialog.FileNames)
                {
                    lstSendFiles.Items.Insert(0, filename);
                }
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {

        }
        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            lstSendFiles.SelectAll();
            foreach (String file in lstSendFiles.SelectedItems)
            {
                int fileLength = file.ToString().Length;
                String message = "";
                message = addPropertyToMessage(message, "filePath", file);
                message = addPropertyToMessage(message, "catg", txtCategory.Text.Trim());
                message = addPropertyToMessage(message, "fromPort", "8080");
                message = addPropertyToMessage(message, "clntID", "clnt1");
                message = addPropertyToMessage(message, "toPort", "8082");
                message = addPropertyToMessage(message, "catg", txtCategory.Text.Trim());
                message = addPropertyToMessage(message, "MSGTYPE", "fPth");
                shim.PostMessage(message);
            }
        }

        private string addPropertyToMessage(string message, string v1, string v2)
        {
            return message += "<" + v1 + ">" + v2 + "</" + v1 + ">";
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            btnAnalyze.Content = "Publishing(Loading)";
            btnAnalyze.IsEnabled = false;
            btnSendFiles.Content = "Downloading(wait)";
            btnSendFiles.IsEnabled = false;

            String message = "";
            message = addPropertyToMessage(message, "command", "1");
            message = addPropertyToMessage(message, "fromPort", "8080");
            message = addPropertyToMessage(message, "clntID", "clnt1");
            message = addPropertyToMessage(message, "toPort", "8082");
            message = addPropertyToMessage(message, "MSGTYPE", "text");
            shim.PostMessage(message);
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            lstSendFiles.Items.Clear();
        }
        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void Button_Click_5(object sender, RoutedEventArgs e)
        {
            lstSendFiles.SelectAll();
            foreach (String file in lstSendFiles.SelectedItems)
            {
                int fileLength = file.ToString().Length;
                String message = "";
                message = addPropertyToMessage(message, "filePath", file);
                message = addPropertyToMessage(message, "catg", txtCategory.Text.Trim());
                message = addPropertyToMessage(message, "fromPort", "8080");
                message = addPropertyToMessage(message, "clntID", "clnt1");
                message = addPropertyToMessage(message, "toPort", "8082");
                message = addPropertyToMessage(message, "catg", txtCategory.Text.Trim());
                message = addPropertyToMessage(message, "MSGTYPE", "fPth");
                shim.PostMessage(message);
            }
        }
        private void btnRefrehServerLst_Click(object sender, RoutedEventArgs e)
        {
            lstServerFiles.Items.Clear();
            cbCategories.Items.Clear();
            sendcatRefreshCommand();


        }
        private void sendcatRefreshCommand()
        {
            String message = "";
            message = addPropertyToMessage(message, "command", "2");//refreshCategories  command
            message = addPropertyToMessage(message, "fromPort", "8080");
            message = addPropertyToMessage(message, "clntID", "clnt1");
            message = addPropertyToMessage(message, "toPort", "8082");
            message = addPropertyToMessage(message, "MSGTYPE", "text");// say that is is a text
            shim.PostMessage(message);
        }
    }
}
