using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Penta.IsignEeWin.Common
{
    public class IniFile
    {
        private string filePath;
        private const int readBufferSize = 255;

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool WritePrivateProfileString(string lpAppName,
            string lpKeyName, string lpString, string lpFileName);

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
        static extern uint GetPrivateProfileString(string lpAppName,
            string lpKeyName, string lpDefault, 
            StringBuilder lpReturnedString, uint nSize,string lpFileName);

        public IniFile(string iniFilePath)
        {
            filePath = iniFilePath;
        }

        public void WriteValue(string section, string key, string value)
        {
            if (WritePrivateProfileString(section, key, value, this.filePath))
                throw new Win32Exception((int)Win32ErrorCode.Succees);
        }
        public string ReadValue(string section, string key)
        {
            StringBuilder temp = new StringBuilder(readBufferSize);
            GetPrivateProfileString(section, key, "", temp, readBufferSize, this.filePath);
            return temp.ToString();
        }
    }


}
