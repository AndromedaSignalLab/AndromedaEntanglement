/*
MacFileAssociationUtil class definitions of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include <vector>
#include "MacFileAssociationUtil.hpp"

std::string Andromeda::Entanglement::MacFileAssociationUtil::queryDescription(CFStringRef uti) {
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

Andromeda::Entanglement::MacAssociationRoles Andromeda::Entanglement::MacFileAssociationUtil::lsRoleMask2macAssociationRoles(const LSRolesMask &lsRoleMask) {
    MacAssociationRoles roles;

    if(lsRoleMask & kLSRolesViewer)
        roles.viewer = true;

    if (lsRoleMask & kLSRolesEditor)
            roles.editor = true;

    if (lsRoleMask & kLSRolesShell)
        roles.shell = true;
    /*
    if (lsRoleMask & kLSRolesAll)
        roles.all = true;

    if (lsRoleMask & kLSRolesNone)
        roles.none = true;
    */
    return roles;
}

LSRolesMask Andromeda::Entanglement::MacFileAssociationUtil::macAssociationRoles2LsRoleMask(const MacAssociationRoles &roles) {
    LSRolesMask roleMask = 0;

    if (roles.viewer)
        roleMask |= kLSRolesViewer;

    if (roles.editor)
        roleMask |= kLSRolesEditor;

    if (roles.shell)
        roleMask |= kLSRolesShell;
    /*
    if (roles.all)
        roleMask |= kLSRolesAll;

    if (roles.none)
        roleMask |= kLSRolesNone;
    */
    return roleMask;
}

Andromeda::Entanglement::MacAssociationRole Andromeda::Entanglement::MacFileAssociationUtil::lsRoleMask2macAssociationRole(const LSRolesMask &lsRoleMask) {
    MacAssociationRole macAssociationRole;
    switch (lsRoleMask) {
        case kLSRolesViewer:
            macAssociationRole = MacAssociationRole::Viewer;
            break;
        case kLSRolesEditor:
            macAssociationRole = MacAssociationRole::Editor;
            break;
        case kLSRolesShell:
            macAssociationRole = MacAssociationRole::Shell;
            break;
        /*
        case kLSRolesAll:
            macAssociationRole = MacAssociationRole::All;
            break;
        case kLSRolesNone:
            macAssociationRole = MacAssociationRole::None;
            break;
        */
    }
    return macAssociationRole;
}

LSRolesMask Andromeda::Entanglement::MacFileAssociationUtil::macAssociationRole2LsRoleMask(const MacAssociationRole &macAssociationRole) {
    LSRolesMask roleMask;
    switch (macAssociationRole) {
        case MacAssociationRole::Viewer:
            roleMask = kLSRolesViewer;
            break;
        case MacAssociationRole::Editor:
            roleMask = kLSRolesEditor;
            break;
        case MacAssociationRole::Shell:
            roleMask = kLSRolesShell;
            break;
        /*
        case MacAssociationRole::All:
            roleMask = kLSRolesAll;
            break;
        case MacAssociationRole::None:
            roleMask = kLSRolesNone;
            break;
        */
    }
    return roleMask;
}

Andromeda::Entanglement::MacAssociationRole
Andromeda::Entanglement::MacFileAssociationUtil::queryRoleFromBundle(
    NSBundle* appBundle,
    NSString* extension,
    CFStringRef uti)
{
    MacAssociationRoles roles;

    NSArray* documentTypes =
        [appBundle objectForInfoDictionaryKey:
            @"CFBundleDocumentTypes"];

    if (!documentTypes)
    {
        return MacAssociationRole::None;
    }

    for (NSDictionary* documentType in documentTypes)
    {
        bool matched = false;

        //
        // Prefer UTI matching
        //
        NSArray* contentTypes =
            documentType[@"LSItemContentTypes"];

        if (contentTypes)
        {
            matched =
                [contentTypes containsObject:
                    (__bridge NSString*)uti];
        }

        //
        // Fallback to extension matching
        //
        if (!matched)
        {
            NSArray* extensions =
                documentType[@"CFBundleTypeExtensions"];

            if (extensions)
            {
                matched =
                    [extensions containsObject:
                        extension];
            }
        }

        if (!matched)
        {
            continue;
        }

        NSString* role =
            documentType[@"CFBundleTypeRole"];

        if (!role)
        {
            continue;
        }

        if ([role isEqualToString:@"Viewer"])
        {
            return MacAssociationRole::Viewer;
        }
        else if ([role isEqualToString:@"Editor"])
        {
            return MacAssociationRole::Editor;
        }
        else if ([role isEqualToString:@"Shell"])
        {
            return MacAssociationRole::Shell;
        }
    }

    return MacAssociationRole::None;
}