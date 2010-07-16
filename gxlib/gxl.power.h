/*
 * This file is a part of Pocket Heroes Game project
 * 	http://www.pocketheroes.net
 *	https://code.google.com/p/pocketheroes/
 *
 * Copyright 2004-2010 by Robert Tarasov and Anton Stuk (iO UPG)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ 

#ifndef _GXL_POWER_MANAGER_H_
#define _GXL_POWER_MANAGER_H_

class iLightHolder
{
public:
	iLightHolder() { HoldBacklight(); }
	~iLightHolder() { RestoreBacklight(); }

private:
	
	void HoldBacklight() 
	{
		HKEY key;
		DWORD dwDesc = REG_OPENED_EXISTING_KEY;
		if (RegOpenKeyEx(HKEY_CURRENT_USER,TEXT("ControlPanel\\Backlight"), 0,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS) {
			DWORD dwType = REG_DWORD;
			DWORD tmpT = 0x7FFFFFFF;
			DWORD dwS;

			RegQueryValueEx(key,TEXT("BatteryTimeout"),0,&dwType,(PBYTE)&m_orgBtTimeout,&dwS);
			RegQueryValueEx(key,TEXT("ACTimeout"),0,&dwType,(PBYTE)&m_orgAcTimeout,&dwS);

			RegSetValueEx(key,TEXT("BatteryTimeout"),0,dwType,(PBYTE)&tmpT,4);
			RegSetValueEx(key,TEXT("ACTimeout"),0,dwType,(PBYTE)&tmpT,4);
			RegCloseKey(key);

			HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("BackLightChangeEvent"));

			if (hEvent) {
				SetEvent(hEvent);
				CloseHandle(hEvent);
			}
		}
	}

	void RestoreBacklight() 
	{
		HKEY key;

		if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("ControlPanel\\Backlight"), 0,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS) {

			DWORD dwType = REG_DWORD;
			RegSetValueEx(key,TEXT("BatteryTimeout"),0,dwType,(PBYTE)&m_orgBtTimeout,4);
			RegSetValueEx(key,TEXT("ACTimeout"),0,dwType,(PBYTE)&m_orgAcTimeout,4);
			RegCloseKey(key);

			HANDLE hEvent =	CreateEvent(NULL, FALSE, FALSE, TEXT("BackLightChangeEvent"));
			if (hEvent) {
				SetEvent(hEvent);
				CloseHandle(hEvent);
			}
		}
	}

private:
	DWORD	m_orgBtTimeout;
	DWORD	m_orgAcTimeout;
};

#endif //_GXL_POWER_MANAGER_H_

