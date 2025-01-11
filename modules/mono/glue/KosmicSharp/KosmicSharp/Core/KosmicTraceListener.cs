using System.Diagnostics;

namespace Kosmic
{
    internal class KosmicTraceListener : TraceListener
    {
        public override void Write(string message)
        {
            KS.PrintRaw(message);
        }

        public override void WriteLine(string message)
        {
            KS.Print(message);
        }

        public override void Fail(string message, string detailMessage)
        {
            KS.PrintErr("Assertion failed: ", message);
            KS.PrintErr("  Details: ", detailMessage);

            try
            {
                string stackTrace = new StackTrace(true).ToString();
                KS.PrintErr(stackTrace);
            }
            catch
            {
                // ignored
            }
        }
    }
}
