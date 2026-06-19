/*
EntanglementObjects class declarations of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#pragma once
#include <string>
#include <optional>
#include <vector>
#include <cstddef>

namespace Andromeda::Entanglement {
    struct MacAssociationRoles {
        bool viewer = false;
        bool editor = false;
        bool shell = false;
        //bool all = false;
        //bool none = false;
    };

    enum class MacAssociationRole : unsigned int {
        None = 0x00000001,
        Viewer = 0x00000002,
        Editor = 0x00000004,
        Shell = 0x00000008,
        //All = (unsigned int) 0xFFFFFFFF
    };

    enum class AssociationScope {
        CurrentUser,
        System
    };

    inline constexpr std::array MacAssociationRolesList {
        MacAssociationRole::Viewer,
        MacAssociationRole::Editor,
        MacAssociationRole::Shell
    };

    struct MacAssociationDetails {
        std::string uti;
        MacAssociationRole associationRole;
        std::string bundleIdentifier;
    };

    struct WindowsAssociationDetails {
        std::string progId;
        std::string applicationId;
        std::string executablePath;
        AssociationScope scope;
    };

    struct LinuxAssociationDetails {
        std::string desktopFile;
    };

    struct ApplicationInfo {
        std::string applicationName;
    };

    enum class IconSource {
        None,
        FilePath,
        BinaryData
    };

    struct IconInfo {
        IconSource iconSource = IconSource::None;
        std::optional<std::string> iconName;
        std::optional<std::string> iconPath;
        std::vector<std::byte> iconData;
    };

    struct FileTypeInfo {
        std::string extension;
        std::string mimeType;
        std::string description;
        IconInfo iconInfo;
    };

    struct FileAssociation {
        ApplicationInfo applicationInfo;
        FileTypeInfo fileTypeInfo;

        bool handledByCurrentApplication = false;
        bool associated = false;

        std::optional<MacAssociationDetails> macDetails;
        std::optional<WindowsAssociationDetails> windowsDetails;
        std::optional<LinuxAssociationDetails> linuxDetails;
    };

    struct FileAssociationRequest {
        std::string extension;
    };

    struct MacFileAssociationRequest : FileAssociationRequest, MacAssociationDetails {
    };

}
