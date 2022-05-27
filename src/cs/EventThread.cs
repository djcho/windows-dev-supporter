using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.AccessControl;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Penta.IsignEeWin.Common
{
    public class EventThread
    {
        private bool abort;
        private bool createdNew;
        private Thread eventThread;
        private EventWaitHandle handle;
        
        public event EventHandler ReceiveSignal;
        int Timeout { get; set; }
        
        private EventThread()
        {
            this.abort = false;
            this.eventThread = new Thread(DoWork);
        }

        public EventThread(bool initalState, EventResetMode mode, string eventName, EventWaitHandleSecurity security, int millisecondsWaitTimeout)
            : this()
        {
            handle = new EventWaitHandle(initalState, mode, eventName, out createdNew, security);
            this.eventThread.Start();
            this.Timeout = millisecondsWaitTimeout;
        }

        public EventThread(EventWaitHandle eventHandle, int millisecondsWaitTimeout)
            : this()
        {
            this.handle = eventHandle;
            this.eventThread.Start();
            this.Timeout = millisecondsWaitTimeout;
        }

        void DoWork()
        {
            bool signal = false;
            while (true)
            {
                signal = handle.WaitOne(Timeout);
                if (abort)
                    return;

                if(signal && 
                    ReceiveSignal != null)
                {
                    ReceiveSignal(this, null);
                }
            }
        }

        public void Abort()
        {
            abort = false;
            handle.Set();

            Thread.Sleep(100);
        }

        public void Close()
        {
            Abort();
            handle.Close();
        }
    }
}
