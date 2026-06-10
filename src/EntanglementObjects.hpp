//
// Created by Volkan Orhan on 9.06.2026.
//
#pragma once
#include <string>
#include <optional>
#include <vector>
#include <cstddef>

namespace Andromeda::Entanglement {
    enum class MacRole {
        Viewer,
        Editor,
        Shell,
        All,
        Unknown
    };

    struct MacAssociationDetails
    {
        std::string uti;
        MacRole role = MacRole::Unknown;
        std::string bundleIdentifier;
        std::string launchServicesIdentifier;
    };

    struct WindowsAssociationDetails
    {
        std::string progId;
        std::string perceivedType;
    };

    struct LinuxAssociationDetails
    {
        std::string desktopFile;
    };

    struct ApplicationInfo
    {
        std::string applicationName;
    };

    enum class IconSource
    {
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

    struct FileTypeInfo
    {
        std::string extension;
        std::string mimeType;
        std::string description;
        IconInfo iconInfo;
    };

    struct FileAssociation
    {
        ApplicationInfo applicationInfo;
        FileTypeInfo fileTypeInfo;

        bool handledByCurrentApplication = false;
        bool associated = false;

        std::optional<MacAssociationDetails> macDetails;
        std::optional<WindowsAssociationDetails> windowsDetails;
        std::optional<LinuxAssociationDetails> linuxDetails;
    };
}
