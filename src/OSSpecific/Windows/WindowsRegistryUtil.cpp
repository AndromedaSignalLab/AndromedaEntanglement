/*
WindowsRegistryUtil class definitions of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "WindowsRegistryUtil.hpp"

#include <windows.h>

#include <stdexcept>
#include <vector>

#include <UnicodeUtil.hpp>

using namespace Andromeda::Entanglement;

std::vector<WindowsProgIdInfo> WindowsRegistryUtil::getProgIds(const std::string& extension) {
    std::vector<WindowsProgIdInfo> result;

    if (auto progId = getProgId(
            extension,
            WindowsAssociationScope::CurrentUser))
    {
        result.push_back({
            WindowsAssociationScope::CurrentUser,
            *progId
        });
    }

    if (auto progId = getProgId(
            extension,
            WindowsAssociationScope::AllUsers))
    {
        result.push_back({
            WindowsAssociationScope::AllUsers,
            *progId
        });
    }

    return result;
}

HKEY WindowsRegistryUtil::getRootKey(const WindowsAssociationScope& scope) {
    switch (scope) {
        case WindowsAssociationScope::CurrentUser:
            return HKEY_CURRENT_USER;
        case WindowsAssociationScope::AllUsers:
            return HKEY_LOCAL_MACHINE;
    }

    return nullptr;
}

std::optional<std::string> WindowsRegistryUtil::getProgId(const std::string& extension, const WindowsAssociationScope& scope) {
    HKEY rootKey = getRootKey(scope);

    if (rootKey == nullptr)
        return std::nullopt;

    std::wstring ext = normalizeExtension(extension);

    if (ext.empty())
        return std::nullopt;

    std::wstring subKey = L"Software\\Classes\\" + ext;

    auto progId = readStringValue(rootKey, subKey);

    if (!progId)
        return std::nullopt;

    return UnicodeUtil::wideToUtf8(*progId);
}

std::optional<std::wstring> WindowsRegistryUtil::readStringValue(HKEY rootKey, const std::wstring& subKey, const std::wstring& valueName) {
    HKEY hKey = nullptr;

    LONG result = RegOpenKeyExW(
        rootKey,
        subKey.c_str(),
        0,
        KEY_READ,
        &hKey);

    if (result != ERROR_SUCCESS)
        return std::nullopt;

    DWORD type = 0;
    DWORD size = 0;

    result = RegQueryValueExW(
        hKey,
        valueName.empty() ? nullptr : valueName.c_str(),
        nullptr,
        &type,
        nullptr,
        &size);

    if (result != ERROR_SUCCESS || type != REG_SZ)
    {
        RegCloseKey(hKey);
        return std::nullopt;
    }

    std::vector<wchar_t> buffer(size / sizeof(wchar_t));

    result = RegQueryValueExW(
        hKey,
        valueName.empty() ? nullptr : valueName.c_str(),
        nullptr,
        nullptr,
        reinterpret_cast<LPBYTE>(buffer.data()),
        &size);

    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS)
        return std::nullopt;

    return std::wstring(buffer.data());
}

std::wstring WindowsRegistryUtil::normalizeExtension(const std::string& extension) {
    if (extension.empty())
        return {};

    std::string ext = extension;

    if (ext.front() != '.')
        ext.insert(ext.begin(), '.');

    return UnicodeUtil::utf8ToWide(ext);
}

std::optional<std::string> WindowsRegistryUtil::getUserChoiceProgId(const std::string& extension) {
    std::wstring ext = normalizeExtension(extension);

    if (ext.empty())
        return std::nullopt;

    std::wstring subKey =
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\" +
        ext +
        L"\\UserChoice";

    auto progId = readStringValue(
        HKEY_CURRENT_USER,
        subKey,
        L"ProgId");

    if (!progId)
        return std::nullopt;

    return UnicodeUtil::wideToUtf8(*progId);
}

std::wstring WindowsRegistryUtil::verbToString(const WindowsVerb verb) {
    switch (verb)
    {
        case WindowsVerb::Open:
            return L"open";

        case WindowsVerb::Edit:
            return L"edit";

        case WindowsVerb::Print:
            return L"print";

        case WindowsVerb::Play:
            return L"play";

        case WindowsVerb::Preview:
            return L"preview";

        case WindowsVerb::RunAs:
            return L"runas";

        default:
            throw std::invalid_argument("Unknown WindowsVerb.");
    }
}

std::optional<std::string> WindowsRegistryUtil::getCommand(const std::string& progId, const WindowsAssociationScope& scope, const WindowsVerb verb) {
    if (progId.empty())
        return std::nullopt;

    HKEY rootKey = getRootKey(scope);

    if (rootKey == nullptr)
        return std::nullopt;

    std::wstring subKey =
        L"Software\\Classes\\" +
        UnicodeUtil::utf8ToWide(progId) +
        L"\\shell\\" +
        verbToString(verb) +
        L"\\command";

    auto command = readStringValue(rootKey, subKey);

    if (!command)
        return std::nullopt;

    return UnicodeUtil::wideToUtf8(*command);
}

std::optional<WindowsIconDetails> WindowsRegistryUtil::getDefaultIcon(const std::string& progId, const WindowsAssociationScope& scope) {
    if (progId.empty())
        return std::nullopt;

    HKEY rootKey = getRootKey(scope);

    if (rootKey == nullptr)
        return std::nullopt;

    std::wstring subKey =
        L"Software\\Classes\\" +
        UnicodeUtil::utf8ToWide(progId) +
        L"\\DefaultIcon";

    auto icon = readStringValue(rootKey, subKey);

    if (!icon)
        return std::nullopt;

    return parseIconLocation(UnicodeUtil::wideToUtf8(*icon));
}

std::optional<WindowsIconDetails> WindowsRegistryUtil::parseIconLocation(const std::string& iconLocation) {
    if (iconLocation.empty())
        return std::nullopt;

    auto separator = iconLocation.rfind(',');

    WindowsIconDetails details;

    if (separator == std::string::npos) {
        details.resourcePath = iconLocation;
        return details;
    }

    details.resourcePath = iconLocation.substr(0, separator);

    try {
        details.iconIdentifier = std::stoi(iconLocation.substr(separator + 1));
    }
    catch (...) {
        return std::nullopt;
    }

    return details;
}