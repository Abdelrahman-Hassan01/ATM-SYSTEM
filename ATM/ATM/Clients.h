#pragma once
#include "ATM.h"

bool LoadClientInfo(string AccountNumber, string PinCode);
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#");
string ConvertRecordToLine(sClient ClientRecord , string Seperator = "#//#");
vector<sClient> LoadCleintsDataFromFile(string FileName);
void SaveClientDataToFile(string FileName, vector<sClient>& vClient);
