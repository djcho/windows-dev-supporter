using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;

namespace Penta.IsignEeWin.Common
{
    public class Win32Security
    {
        public static EventWaitHandleSecurity GetEventWaitHandleOfUsersSecurity()
        {
            /*
            SecurityIdentifier users = new SecurityIdentifier(WellKnownSidType.BuiltinUsersSid, null);
            EventWaitHandleAccessRule rule = new EventWaitHandleAccessRule(users, EventWaitHandleRights.Synchronize | EventWaitHandleRights.Modify,
                                      AccessControlType.Allow);
            EventWaitHandleSecurity security = new EventWaitHandleSecurity();
            security.AddAccessRule(rule);
            */


            SecurityIdentifier sid = new SecurityIdentifier(WellKnownSidType.WorldSid, null);
            EventWaitHandleSecurity security = new EventWaitHandleSecurity();
            security.AddAccessRule(new EventWaitHandleAccessRule(sid, EventWaitHandleRights.FullControl, AccessControlType.Allow));
            security.AddAccessRule(new EventWaitHandleAccessRule(sid, EventWaitHandleRights.ChangePermissions, AccessControlType.Deny));
            return security;
        }
    }
}
