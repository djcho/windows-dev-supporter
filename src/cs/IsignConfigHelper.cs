using log4net;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Penta.IsignEeWin.Common
{


    internal class IsignConfigHelperDefinitions
    {
        internal const string registryKeyRoot = @"SOFTWARE\Penta Security Systems\ISign\EE-WIN";
        internal const string registryKeyISignSsoCs = @"SOFTWARE\Microsoft\Internet Explorer\LowRegistry\Penta Security Systems\IsignPlusSSO\CS";
        internal const string registryKeyLogonManager = @"SOFTWARE\Penta Security Systems\ISign\EE-WIN\LogonManager";
        internal const string registryKeyConfig = @"SOFTWARE\Penta Security Systems\ISign\EE-WIN\Config";
        internal const string registryKeyLogonUI = @"SOFTWARE\Microsoft\Windows\CurrentVersion\Authentication\LogonUI";
        internal const string registryNameLoginState = "LoginState";
        internal const string registryNameSecureToken = "SecureToken";
        internal const string registryNameProductVersion = "Version";
        internal const string registryNameInstallationPath = "InstallDir";
        internal const string registryNameLastLoggedOnSsoId = "LastLoggedOnSSOID";
        internal const string registryNameMotpDeptAdminPageUrlForPC = "MotpDeptAdminPageUrl_PC";
        internal const string registryNameMotpDeptAdminPageUrlForVM = "MotpDeptAdminPageUrl_VM";
        internal const string registryNameSmartAuthCenterPageUrl = "SmartAuthCenterPageUrl";
        internal const string registryNameRegisterFingerPrintPageUrl = "RegisterFingerPrintPageUrl";
        internal const string registryNameIsVmModel = "IsVmModel";
        internal const string registryNameCertificateStatus = "CertificateStatus";
        internal const string registryNameLastLoggedOnProvider = "LastLoggedOnProvider";
    }

    public enum LoginState
    {
        Unknown = 0,
        Logout,
        Login
    };

    public class IsignConfigHelper
    {
        private static readonly ILog logger = LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

        public static string getInstallationPath()
        {
            string installDir = string.Empty;
            RegistryKey regKey = null;

            try
            {
                regKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
                    .OpenSubKey(IsignConfigHelperDefinitions.registryKeyRoot, false);
                installDir = regKey.GetValue(IsignConfigHelperDefinitions.registryNameInstallationPath) as string;
            }
            catch(Exception ex)
            {
                logger.Error(ex.ToString());
            }

            if (regKey != null)
                regKey.Dispose();

            return installDir;
        }

        public static string getLastLoggedOnSsoId()
        {
            string ssoId = string.Empty;
            RegistryKey regKey = null;

            try
            {
                regKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
               .OpenSubKey(IsignConfigHelperDefinitions.registryKeyLogonManager, false);

                ssoId = regKey.GetValue(IsignConfigHelperDefinitions.registryNameLastLoggedOnSsoId) as string;
            }
            catch (Exception ex)
            {
                logger.Error(ex.ToString());
            }

            if (regKey != null)
                regKey.Dispose();

            return ssoId;
        }

        public static LoginState getLoginStateFromRegistry()
        {
            LoginState loginState = LoginState.Logout;
            RegistryKey regKey = null;
            try
            {
                regKey = RegistryKey.OpenBaseKey(RegistryHive.CurrentUser, RegistryView.Registry64)
                .OpenSubKey(IsignConfigHelperDefinitions.registryKeyISignSsoCs, false);

                if (regKey == null)
                {
                    logger.Error("regKey is NULL");
                    return loginState;
                }

                int isignSsoState = int.Parse(regKey.GetValue(IsignConfigHelperDefinitions.registryNameLoginState).ToString());
                string token = regKey.GetValue(IsignConfigHelperDefinitions.registryNameSecureToken) == null ? "" : regKey.GetValue(IsignConfigHelperDefinitions.registryNameSecureToken).ToString();
                regKey.Dispose();

                regKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
                .OpenSubKey(IsignConfigHelperDefinitions.registryKeyLogonManager, false);
                int certState = int.Parse(regKey.GetValue(IsignConfigHelperDefinitions.registryNameCertificateStatus).ToString());
                regKey.Dispose();

                if (isignSsoState == 2  && token.Length > 0 && certState == 0)
                {
                    loginState = LoginState.Login;
                }
            }
            catch (Exception ex)
            {
                if (regKey != null)
                    regKey.Dispose();

                loginState = LoginState.Logout;
                logger.Error(ex.ToString());
            }

            return loginState;
        }

        public static string getProductVersion()
        {
            string productVersion = string.Empty; ;
            RegistryKey regKey = null;
            try
            {
                regKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
                    .OpenSubKey(IsignConfigHelperDefinitions.registryKeyRoot, false);
                productVersion = regKey.GetValue(IsignConfigHelperDefinitions.registryNameProductVersion) as string;
            }
            catch (Exception ex)
            {
                logger.Error(ex.ToString());
            }

            if(regKey != null)
                regKey.Dispose();

            return productVersion;
        }

        public static string getMotpDeptAdminPageUrl()
        {
            string motpDeptAdminPageUrl = string.Empty;
            RegistryKey regKey = null;
            try
            {
                regKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
                    .OpenSubKey(IsignConfigHelperDefinitions.registryKeyConfig, false);
                if(isVmModel())
                    motpDeptAdminPageUrl = regKey.GetValue(IsignConfigHelperDefinitions.registryNameMotpDeptAdminPageUrlForVM) as string;
                else
                    motpDeptAdminPageUrl = regKey.GetValue(IsignConfigHelperDefinitions.registryNameMotpDeptAdminPageUrlForPC) as string;
            }
            catch (Exception ex)
            {
                logger.Error(ex.ToString());
            }

            if (regKey != null)
                regKey.Dispose();

            return motpDeptAdminPageUrl;
        }

        public static string getRegisterFingerPrintPageUrl()
        {
            string registerFingerPrintPageUrl = string.Empty;
            RegistryKey regKey = null;
            try
            {
                regKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
                    .OpenSubKey(IsignConfigHelperDefinitions.registryKeyConfig, false);
                registerFingerPrintPageUrl = regKey.GetValue(IsignConfigHelperDefinitions.registryNameRegisterFingerPrintPageUrl) as string;
            }
            catch (Exception ex)
            {
                logger.Error(ex.ToString());
            }

            if (regKey != null)
                regKey.Dispose();

            return registerFingerPrintPageUrl;
        }
        public static string getSmartAuthCenterPageUrl()
        {
            string smartAuthCenterPageUrl = string.Empty;
            RegistryKey regKey = null;
            try
            {
                regKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
                    .OpenSubKey(IsignConfigHelperDefinitions.registryKeyConfig, false);
                smartAuthCenterPageUrl = regKey.GetValue(IsignConfigHelperDefinitions.registryNameSmartAuthCenterPageUrl) as string;
            }
            catch (Exception ex)
            {
                logger.Error(ex.ToString());
            }

            if (regKey != null)
                regKey.Dispose();

            return smartAuthCenterPageUrl;
        }

        public static bool isVmModel()
        {
            string isVmModel = string.Empty;
            RegistryKey regKey = null;
            try
            {
                regKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
                    .OpenSubKey(IsignConfigHelperDefinitions.registryKeyRoot, false);
                if(regKey != null)
                    isVmModel = regKey.GetValue(IsignConfigHelperDefinitions.registryNameIsVmModel).ToString();
            }
            catch (Exception ex)
            {
                logger.Error(ex.ToString());
                return false;
            }

            if (regKey != null)
                regKey.Dispose();

            return Convert.ToInt32(isVmModel) == 1 ? true : false;
        }

        public static string getLastLoggedOnProvider()
        {
            string lastLoggedOnProvider = string.Empty;
            RegistryKey regKey = null;
            try
            {
                regKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
                    .OpenSubKey(IsignConfigHelperDefinitions.registryKeyLogonUI, false);
                lastLoggedOnProvider = regKey.GetValue(IsignConfigHelperDefinitions.registryNameLastLoggedOnProvider) as string;
                
            }
            catch (Exception ex)
            {
                logger.Error(ex.ToString());
            }

            if (regKey != null)
                regKey.Dispose();

            return lastLoggedOnProvider;
        }
    }
}
