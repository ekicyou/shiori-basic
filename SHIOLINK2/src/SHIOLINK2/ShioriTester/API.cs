using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace ShioriTester
{
    internal static class API
    {
        private const string SHIORI_DLL = "SHIOLINK.dll";

        #region entry point
        [DllImport(SHIORI_DLL, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern bool load(IntPtr hGlobal_loaddir, int loaddir_len);
        [DllImport(SHIORI_DLL, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern bool unload();
        [DllImport(SHIORI_DLL, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern IntPtr request(IntPtr hGlobal_request, ref int len);
        #endregion

        public static bool Load(string loaddir)
        {
            byte[] buf = Encoding.Default.GetBytes(loaddir);
            IntPtr hLoaddir = Marshal.AllocHGlobal(buf.Length);
            Marshal.Copy(buf, 0, hLoaddir, buf.Length);
            return load(hLoaddir, buf.Length);
        }

        public static bool Unload()
        {
            return unload();
        }

        public static string Request(string req)
        {
            byte[] bReq = Encoding.UTF8.GetBytes(req);
            IntPtr hReq = Marshal.AllocHGlobal(bReq.Length);
            int len = bReq.Length;
            IntPtr hRes = request(hReq, ref len);
            try {
                byte[] bRes = new byte[len];
                Marshal.Copy(hReq, bRes, 0, len);
                string res = Encoding.UTF8.GetString(bRes);
                return res;
            }
            finally {
                Marshal.FreeHGlobal(hRes);
            }
        }
    }
}
