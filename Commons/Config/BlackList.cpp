//
// Copyright(c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "BlackList.h"
#include "..\Utility\ProcessHelper.h"
#include "..\Utility\FileDirectory.h"
#include "..\Logging\MessageLog.h"
#include "..\Utility\StringUtils.h"

#include <fstream>

void BlackList::Load()
{
  if (loaded_) return;

  const std::wstring blackListFile =
    g_fileDirectory.GetDirectory(DirectoryType::Config) + L"blackList.txt";
  std::wifstream file(blackListFile);
  if (file.is_open())
  {
    for (std::wstring line; std::getline(file, line);)
    {
      blackList_.push_back(line);
    }
    g_messageLog.LogInfo("BlackList", "Blacklist file loaded");
  }
  else
  {
    CreateDefault(blackListFile);
  }

  loaded_ = true;
}

bool BlackList::Contains(const std::wstring& value) const
{
  // ignore system processes without names
  if (value.empty()) {
    return true;
  }

  for (auto& entry : blackList_) {
    if (_wcsicmp(entry.c_str(), value.c_str()) == 0) {
      return true;
    }
  }
  return false;
}

void BlackList::CreateDefault(const std::wstring& fileName)
{
  g_messageLog.LogInfo("BlackList", "Create default blacklist file");

  blackList_ = { L"Steam.exe", L"Origin.exe", L"GalaxyClient.exe", L"Battle.net.exe",
      L"OCAT.exe", L"firefox.exe", L"RadeonSettings.exe", L"dwm.exe",
    // Uplay
      L"upc.exe", L"Uplay.exe", L"UplayWebCore.exe", L"UbisoftGameLauncher.exe" };

  std::wofstream file(fileName);
  if (file.is_open())
  {
    for (auto& value : blackList_)
    {
      file << value << std::endl;
    }
  }
  else
  {
    g_messageLog.LogError("BlackList", "Unable to create default blacklist file");
  }
}