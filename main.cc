#include <Windows.h>
#include <UIAutomation.h>
#include <iostream>
#include <string>
#include <comip.h>
#include <vector>
#include <regex>
#include <WinUser.h>
#include <cctype>
#include <algorithm>

using namespace std;


typedef _com_ptr_t<_com_IIID<IUIAutomation, &__uuidof(IUIAutomation)>> IUIAutomationPtr;
typedef _com_ptr_t<_com_IIID<IUIAutomationElement, &__uuidof(IUIAutomationElement)>> IUIAutomationElementPtr;
typedef _com_ptr_t<_com_IIID<IUIAutomationCondition, &__uuidof(IUIAutomationCondition)>> IUIAutomationConditionPtr;
typedef _com_ptr_t<_com_IIID<IUIAutomationElementArray, &__uuidof(IUIAutomationElementArray)>> IUIAutomationElementArrayPtr;
typedef _com_ptr_t<_com_IIID<IUIAutomationInvokePattern, &__uuidof(IUIAutomationInvokePattern)>> IUIAutomationInvokePatternPtr;

const wchar_t * TaskBarName = L"任务栏";
const char * ImeNamePrefix = "托盘输入指示器";

string get_element_name(IUIAutomationElementPtr pElement)
{
  _bstr_t name;
  pElement->get_CurrentName(name.GetAddress());
  if (name.length() > 0)
  {
    return string((const char*)name);
  }
  else
  {
    return "";
  }
}

vector<string> split_string(const string & str, const string & delim)
{
  vector<string> result;
  regex re(delim);
  sregex_token_iterator first{ str.begin(), str.end(), re, -1 }, last;
  return { first, last };
}

SHORT vk_from_text(const string & text) {
  if (text == "shift") {
    return VK_SHIFT;
  }
  if (text == "ctrl") {
    return VK_CONTROL;
  }
  if (text == "alt") {
    return VK_MENU;
  }
  if (text == "win") {
    return VK_LWIN;
  }
  if (text == "space") {
    return VK_SPACE;
  }
  return 0;
}

vector<INPUT> get_input_from_string(string str)
{
  vector<INPUT> result;
  transform(str.begin(), str.end(), str.begin(), ::tolower);
  auto keys = split_string(str, "\\+");
  transform(keys.begin(), keys.end(), back_insert_iterator(result), [](const string & key) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk_from_text(key);
    return input;
  });
  transform(keys.rbegin(), keys.rend(), back_insert_iterator(result), [](const string & key) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk_from_text(key);
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    return input;
  });
  return result;
}


struct ImeButton
{
  string current_mode;
  IUIAutomationElementPtr pElement;
};


ImeButton get_ime_button(IUIAutomationPtr pAutomation) {
  IUIAutomationElementPtr pDesktop;
  IUIAutomationElementPtr pTaskBar;

  pAutomation->GetRootElement(&pDesktop);

  IUIAutomationConditionPtr pCondition;
  pAutomation->CreatePropertyCondition(UIA_NamePropertyId, _variant_t(TaskBarName), &pCondition);

  pDesktop->FindFirst(TreeScope_Children, pCondition, &pTaskBar);

  pAutomation->CreatePropertyCondition(UIA_ControlTypePropertyId, _variant_t(UIA_ButtonControlTypeId), &pCondition);

  IUIAutomationElementArrayPtr arrButtons;
  pTaskBar->FindAll(TreeScope_Descendants, pCondition, &arrButtons);

  int length = 0;
  arrButtons->get_Length(&length);
  for (int i = 0; i < length; i++)
  {
    IUIAutomationElementPtr pButton;
    arrButtons->GetElement(i, &pButton);
    auto name = get_element_name(pButton);

    if (name.find(ImeNamePrefix) == 0)
    {
      auto parts = split_string(name, "\\s");
      return { parts[1], pButton };
    }
  }
  return { "", nullptr };
}



int main(int argc, char * argv[])
{
  CoInitialize(NULL);

  IUIAutomationPtr pAutomation;

  auto hr = pAutomation.CreateInstance(CLSID_CUIAutomation);

  if (FAILED(hr))
  {
    return 1;
  }

  auto ime_button = get_ime_button(pAutomation);

  if (!ime_button.pElement)
  {
    return 1;
  }

  if (argc == 1)
  {
    // get current mode
    cout << ime_button.current_mode << endl;
  }
  else
  {
    // do switch
    auto mode = argv[1];
    if (mode != ime_button.current_mode)
    {
      const char * keys = "shift";
      if (argc > 2) {
        keys = argv[2];
      }
      auto input = get_input_from_string(keys);
      SendInput((UINT)input.size(), input.data(), sizeof(input[0]));
    }
  }


  CoUninitialize();
  return 0;
}