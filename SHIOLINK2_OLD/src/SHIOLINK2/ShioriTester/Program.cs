using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace ShioriTester
{
    class Program
    {
        static void Main(string[] args)
        {
            try {
                string loaddir = AssemblyUtil.GetCallingAssemblyDirctory();
                bool rc = API.Load(loaddir);
                

                rc = API.Unload();
            }
            catch (Exception ex) {
                Console.WriteLine(ex);
            }
            finally {
                if (Debugger.IsAttached) {
//                    Console.WriteLine("\n\nHit Any Key.");
//                    Console.Read();
                }
            }
        }
    }
}
