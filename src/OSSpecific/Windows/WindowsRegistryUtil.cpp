/*
WindowsRegistryUtil class definitions of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "WindowsRegistryUtil.hpp"
#include <windows.h>
#include <vector>

std::optional<std::string> WindowsRegistryUtil::getProgId(const std::string& extension) {
    HKEY hKey = nullptr;

    std::string ext = extension;

    if (!ext.empty() && ext.front() != '.')
        extension.insert(ext.begin(), '.');

    std::wstring subKey(ext.begin(), ext.end());

    LONG result = RegOpenKeyExW(
        HKEY_CLASSES_ROOT,
        subKey.c_str(),
        0,
        KEY_READ,
        &hKey
    );

    if (result != ERROR_SUCCESS)
        return std::nullopt;

    DWORD type = 0;
    DWORD size = 0;

    result = RegQueryValueExW(
        hKey,
        nullptr,          // Default value
        nullptr,
        &type,
        nullptr,
        &size
    );

    if (result != ERROR_SUCCESS || type != REG_SZ) {
        RegCloseKey(hKey);
        return std::nullopt;
    }

    std::vector<wchar_t> buffer(size / sizeof(wchar_t));

    result = RegQueryValueExW(
        hKey,
        nullptr,
        nullptr,
        nullptr,
        reinterpret_cast<LPBYTE>(buffer.data()),
        &size
    );

    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS)
        return std::nullopt;

    std::wstring progId(buffer.data());

    return std::string(progId.begin(), progId.end());
}
