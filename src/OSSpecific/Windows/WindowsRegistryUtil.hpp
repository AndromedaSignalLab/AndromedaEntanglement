/*
WindowsRegistryUtil class definitions of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#pragma once

#include <optional>
#include <vector>
#include <string>
#include "../../EntanglementObjects.hpp"

struct HKEY__;
using HKEY = HKEY__*;

 namespace Andromeda::Entanglement {
     class WindowsRegistryUtil {
        public:
            static std::optional<std::string> getProgId(const std::string& extension, const WindowsAssociationScope& scope);
            static std::vector<WindowsProgIdInfo> getProgIds(const std::string& extension);
            static std::optional<std::string> getUserChoiceProgId(const std::string& extension);
            static std::optional<std::string> getCommand(const std::string& progId, const WindowsAssociationScope& scope, const WindowsVerb verb = WindowsVerb::Open);
            //static getDefaultIcon();
     private:
            static HKEY getRootKey(const WindowsAssociationScope& scope);
            static std::optional<std::wstring> readStringValue(HKEY rootKey, const std::wstring& subKey, const std::wstring& valueName = L"");
            static std::wstring normalizeExtension(const std::string& extension);
            static std::wstring verbToString(const WindowsVerb verb);
     };
 };
