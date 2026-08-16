/*
WindowsAssociationUtil class definitions of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "WindowsAssociationUtil.hpp"

#include <windows.h>
#include <shellapi.h>

#include "UnicodeUtil.hpp"

using namespace Andromeda::Entanglement;
#include "WindowsAssociationUtil.hpp"

std::optional<WindowsIconDetails> WindowsAssociationUtil::getAssociatedIcon(const std::string& extension) {
    if (extension.empty())
        return std::nullopt;

    std::wstring ext = UnicodeUtil::utf8ToWide(extension);

    if (ext.front() != L'.')
        ext.insert(ext.begin(), L'.');

    SHFILEINFOW shfi {};

    if (!SHGetFileInfoW(
            ext.c_str(),
            FILE_ATTRIBUTE_NORMAL,
            &shfi,
            sizeof(shfi),
            SHGFI_USEFILEATTRIBUTES | SHGFI_ICONLOCATION))
    {
        return std::nullopt;
    }

    WindowsIconDetails details;
    details.resourcePath = UnicodeUtil::wideToUtf8(shfi.szDisplayName);
    details.iconIdentifier = shfi.iIcon;

    return details;
}