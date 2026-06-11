/*
MacUtils Objective-C class definitions of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include <vector>
#include "MacFileAssociationUtil.hpp"


std::string
Andromeda::Entanglement::MacFileAssociationUtil::queryDescription(CFStringRef uti) {
    CFDictionaryRef declaration =
        UTTypeCopyDeclaration(uti);

    if (!declaration)
    {
        return {};
    }

    std::string result;

    CFStringRef description =
        (CFStringRef)
        CFDictionaryGetValue(
            declaration,
            kUTTypeDescriptionKey);

    if (description)
    {
        result =
            [(NSString*)description UTF8String];
    }

    CFRelease(declaration);

    return result;
}

std::optional<Andromeda::Entanglement::IconInfo> Andromeda::Entanglement::MacFileAssociationUtil::queryDocumentIcon(NSBundle* appBundle, NSString* extension) {
    NSArray* documentTypes =
        [appBundle objectForInfoDictionaryKey:
            @"CFBundleDocumentTypes"];

    if (!documentTypes)
    {
        return std::nullopt;
    }

    for (NSDictionary* documentType in documentTypes)
    {
        NSArray* extensions =
            documentType[@"CFBundleTypeExtensions"];

        if (!extensions)
        {
            continue;
        }

        BOOL matches = NO;

        for (NSString* currentExtension in extensions)
        {
            if ([currentExtension
                    caseInsensitiveCompare:extension]
                == NSOrderedSame)
            {
                matches = YES;
                break;
            }
        }

        if (!matches)
        {
            continue;
        }

        NSString* iconFile =
            documentType[@"CFBundleTypeIconFile"];

        if (!iconFile)
        {
            continue;
        }

        if (![iconFile.pathExtension length])
        {
            iconFile =
                [iconFile stringByAppendingString:@".icns"];
        }

        NSString* resourcesPath =
            [appBundle resourcePath];

        NSString* iconPath =
            [resourcesPath
                stringByAppendingPathComponent:
                    iconFile];

        Andromeda::Entanglement::IconInfo iconInfo;

        iconInfo.iconSource =
            Andromeda::Entanglement::IconSource::FilePath;

        iconInfo.iconName =
            std::string(
                [iconFile UTF8String]);

        iconInfo.iconPath =
            std::string(
                [iconPath UTF8String]);

        return iconInfo;
    }

    return std::nullopt;
}

NSString* Andromeda::Entanglement::MacFileAssociationUtil::currentBundleIdentifier() {
    return [[NSBundle mainBundle]
        bundleIdentifier];
}

Andromeda::Entanglement::MacRole Andromeda::Entanglement::MacFileAssociationUtil::lsRoleMask2macRole(const LSRolesMask &lsRoleMask) {
    MacRole macRole;
    switch (lsRoleMask) {
        case kLSRolesViewer:
            macRole = MacRole::Viewer;
            break;
        case kLSRolesEditor:
            macRole = MacRole::Editor;
            break;
        case kLSRolesShell:
            macRole = MacRole::Shell;
            break;
        case kLSRolesAll:
            macRole = MacRole::All;
            break;
        case kLSRolesNone:
            macRole = MacRole::None;
            break;
    }
    return macRole;
}

LSRolesMask Andromeda::Entanglement::MacFileAssociationUtil::macRole2LsRoleMask(const MacRole &macRole) {
    LSRolesMask roleMask;
    switch (macRole) {
        case MacRole::Viewer:
            roleMask = kLSRolesViewer;
            break;
        case MacRole::Editor:
            roleMask = kLSRolesEditor;
            break;
        case MacRole::Shell:
            roleMask = kLSRolesShell;
            break;
        case MacRole::All:
            roleMask = kLSRolesAll;
            break;
        case MacRole::None:
            roleMask = kLSRolesNone;
            break;
    }
    return roleMask;
}