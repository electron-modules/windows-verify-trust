#define _UNICODE 1
#define UNICODE 1

#include <napi.h>
#include <windows.h>
#include <softpub.h>
#include <wintrust.h>
#include <Libloaderapi.h>
#include <atlstr.h>
#pragma comment (lib, "wintrust")
#pragma comment (lib, "crypt32.lib")

Napi::String verifyInput(const Napi::CallbackInfo& info, const Napi::Env env) {
  if (info.Length() != 1 || !info[0].IsString()) {
    Napi::TypeError::New(env, "Expected one string argument").ThrowAsJavaScriptException();
  }
  Napi::String input = info[0].As<Napi::String>();
  return input;
}

std::wstring napiStringToWString(const Napi::String str) {
  std::string s = str.ToString();
  std::wstring ws = std::wstring(s.begin(), s.end());
  return ws;
}

HMODULE getLibHandle(const Napi::String filename) {
  LPCWSTR wFilename = napiStringToWString(filename).c_str();
  // HMODULE hLib = LoadLibrary(wFilename);
  HMODULE hLib = GetModuleHandle(wFilename);
  return hLib;
}

LPWSTR getModulePath(const HMODULE hLib, const Napi::Env env) {
  LPWSTR path = new WCHAR[MAX_PATH];
  if (GetModuleFileName(hLib, path, MAX_PATH) == 0) {
    Napi::TypeError::New(env, "Module not found").ThrowAsJavaScriptException();
  }
  return path;
}

// check if library exists
Napi::Boolean isLibExist(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String filename = verifyInput(info, env);
  HMODULE hLib = getLibHandle(filename);
  return Napi::Boolean::New(env, hLib != NULL);
}

// get path of library
Napi::String getLibPath(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String filename = verifyInput(info, env);
  HMODULE hLib = getLibHandle(filename);
  if (hLib == NULL) {
    return Napi::String::New(env, "");
  }
  LPWSTR path = getModulePath(hLib, env);
  // LPWSTR to string (using atlstr)
  std::string result = CW2A(path);
  return Napi::String::New(env, result);
}

// verify dll signature
Napi::Number verifySignature(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String filename = verifyInput(info, env);
  
  // get library path
  HMODULE hLib = getLibHandle(filename);
  if (hLib == NULL) {
    Napi::TypeError::New(env, "Library not found").ThrowAsJavaScriptException();
  }
  LPWSTR path = getModulePath(hLib, env);
  
  // setup WinVerifyTrust data structure
  GUID pgActionID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
  WINTRUST_FILE_INFO FileInfo = {};
  WINTRUST_DATA wintrustData = {};

  // WINTRUST_DATA structure: https://learn.microsoft.com/zh-cn/windows/win32/api/wintrust/ns-wintrust-wintrust_data
  wintrustData.cbStruct = sizeof(WINTRUST_DATA);
  wintrustData.dwUIChoice = WTD_UI_NONE;
  wintrustData.dwUnionChoice = WTD_CHOICE_FILE;
  wintrustData.fdwRevocationChecks = WTD_REVOKE_NONE; 
  wintrustData.dwStateAction = WTD_STATEACTION_VERIFY;
  wintrustData.dwUIContext = 0;

  // WINTRUST_FILE_INFO structure: https://learn.microsoft.com/zh-cn/windows/win32/api/wintrust/ns-wintrust-wintrust_file_info
  FileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
  FileInfo.pcwszFilePath = path;

  // set FileInfo as pFile 
  wintrustData.pFile = &FileInfo;

  // verify signature of library
  Napi::Number result;
  LONG lResult = WinVerifyTrust(NULL, &pgActionID, &wintrustData);
  
  // get reason for no signature
  if (lResult == TRUST_E_NOSIGNATURE) {
    result = Napi::Number::New(env, GetLastError());
  } else {
    result = Napi::Number::New(env, lResult);
  }
  
  // cleanup wintrustData and free memory
  wintrustData.dwStateAction = WTD_STATEACTION_CLOSE;
  lResult = WinVerifyTrust(NULL, &pgActionID, &wintrustData);
	
  return result;
}

Napi::Object Init(Napi::Env env, Napi::Object exports){
  exports.Set("isLibExist", Napi::Function::New(env, isLibExist));
  exports.Set("getLibPath", Napi::Function::New(env, getLibPath));
  exports.Set("verifySignature", Napi::Function::New(env, verifySignature));
  return exports;
}

NODE_API_MODULE(verify, Init);
