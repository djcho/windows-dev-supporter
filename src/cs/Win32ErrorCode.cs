using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Penta.IsignEeWin.Common
{
    public enum Win32ErrorCode : long
    {
        Succees = 0L,
        InvalidFunction = 1L,
        FileNotFound = 2L,
        PathNotFound = 3L,
        TooManyOpenFiles = 4L,
        AccessDenied = 5L,
    }
}
