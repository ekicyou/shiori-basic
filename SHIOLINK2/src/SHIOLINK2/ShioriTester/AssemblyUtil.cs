using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Reflection;

namespace ShioriTester
{
    /// <summary>
    /// �A�Z���u�����샆�[�e�B���e�B�B
    /// </summary>
    public static class AssemblyUtil
    {
        /// <summary>
        /// �w�肳�ꂽ�A�Z���u����Path��Ԃ��܂��B
        /// �Ԃ�Path�̓V���h�[�R�s�[�ł͂Ȃ��A�{�̂�Path�ł��B
        /// </summary>
        /// <param name="assembly"></param>
        /// <returns></returns>
        public static string GetAssemblyPath(Assembly assembly)
        {
            Uri binURI = new Uri(assembly.CodeBase);
            return Path.GetFullPath(binURI.AbsolutePath);
        }

        /// <summary>
        /// �Ăяo�����̃A�Z���u����Path��Ԃ��܂��B
        /// �Ԃ�Path�̓V���h�[�R�s�[�ł͂Ȃ��A�{�̂�Path�ł��B
        /// </summary>
        /// <returns></returns>
        public static string GetCallingAssemblyPath()
        {
            return GetAssemblyPath(Assembly.GetCallingAssembly());
        }



        /// <summary>
        /// �w�肳�ꂽ�A�Z���u����DirecryPath��Ԃ��܂��B
        /// �Ԃ�Path�̓V���h�[�R�s�[�ł͂Ȃ��A�{�̂�Path�ł��B
        /// </summary>
        /// <param name="assembly"></param>
        /// <returns></returns>
        public static string GetAssemblyDirctory(Assembly assembly)
        {
            return Path.GetDirectoryName(GetAssemblyPath(assembly));
        }

        /// <summary>
        /// �Ăяo�����̃A�Z���u���� DirecryPath��Ԃ��܂��B
        /// �Ԃ�Path�̓V���h�[�R�s�[�ł͂Ȃ��A�{�̂�Path�ł��B
        /// </summary>
        /// <returns></returns>
        public static string GetCallingAssemblyDirctory()
        {
            return GetAssemblyDirctory(Assembly.GetCallingAssembly());
        }

    }
}
