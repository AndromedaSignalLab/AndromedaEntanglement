/*
FileAssociationManager Objective-C class definitions of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include <vector>
#include <string>
#include <optional>

#include "FileAssociationManager.hpp"
#include "Utils/MacFileAssociationUtil.hpp"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <CoreServices/CoreServices.h>

std::vector<Andromeda::Entanglement::FileAssociation>
Andromeda::Entanglement::FileAssociationManager::queryAssociations(
    const std::vector<std::string>& extensions)
{
    std::vector<FileAssociation> result;

    NSString* currentBundleId =
        [[NSBundle mainBundle] bundleIdentifier];

    NSWorkspace* workspace =
        [NSWorkspace sharedWorkspace];

    for (const auto& ext : extensions)
    {
        FileAssociation association;

        association.fileTypeInfo.extension = ext;

        NSString* nsExt =
            [NSString stringWithUTF8String:
                ext.c_str()];

        CFStringRef uti =
            UTTypeCreatePreferredIdentifierForTag(
                kUTTagClassFilenameExtension,
                (__bridge CFStringRef)nsExt,
                nullptr);

        if (!uti)
        {
            result.push_back(
                std::move(association));

            continue;
        }

        association.macDetails.emplace();

        association.macDetails->uti =
            [(NSString*)uti UTF8String];

        //
        // Description
        //
        auto description =
            MacFileAssociationUtil::queryDescription(uti);

        if (!description.empty())
        {
            association.fileTypeInfo.description =
                std::move(description);
        }
        else
        {
            association.fileTypeInfo.description =
                [(NSString*)uti UTF8String];
        }

        //
        // MIME Type
        //
        CFStringRef mimeType =
            UTTypeCopyPreferredTagWithClass(
                uti,
                kUTTagClassMIMEType);

        if (mimeType)
        {
            association.fileTypeInfo.mimeType =
                [(NSString*)mimeType UTF8String];

            CFRelease(mimeType);
        }

        //
        // Default handler
        //
        CFStringRef handler =
            LSCopyDefaultRoleHandlerForContentType(
                uti,
                kLSRolesAll);

        if (handler)
        {
            association.associated = true;

            NSString* handlerBundleId =
                (__bridge NSString*)handler;

            association.macDetails->bundleIdentifier =
                [handlerBundleId UTF8String];

            association.macDetails->role =
                MacRole::All;

            association.handledByCurrentApplication =
                [handlerBundleId isEqualToString:
                    currentBundleId];

            NSURL* appUrl =
                [workspace
                    URLForApplicationWithBundleIdentifier:
                        handlerBundleId];

            if (appUrl)
            {
                NSBundle* appBundle =
                    [NSBundle bundleWithURL:
                        appUrl];

                NSString* appName =
                    [appBundle
                        objectForInfoDictionaryKey:
                            @"CFBundleName"];

                if (appName)
                {
                    association.applicationInfo.applicationName =
                        [appName UTF8String];
                }

                //
                // Document icon from Info.plist
                //
                auto documentIcon =
                    MacFileAssociationUtil::queryDocumentIcon(
                        appBundle,
                        nsExt);

                if (documentIcon)
                {
                    association.fileTypeInfo.iconInfo =
                        *documentIcon;
                }
                else
                {
                    //
                    // Fallback icon from NSWorkspace
                    //
                    NSImage* icon =
                        [workspace
                            iconForFileType:
                                nsExt];

                    if (icon)
                    {
                        association.fileTypeInfo.iconInfo.iconSource =
                            IconSource::BinaryData;

                        NSData* tiffData =
                            [icon TIFFRepresentation];

                        if (tiffData)
                        {
                            const std::byte* bytes =
                                reinterpret_cast<
                                    const std::byte*>(
                                        [tiffData bytes]);

                            association.fileTypeInfo
                                .iconInfo
                                .iconData
                                .assign(
                                    bytes,
                                    bytes +
                                    [tiffData length]);
                        }
                    }
                }
            }

            CFRelease(handler);
        }

        CFRelease(uti);

        result.push_back(
            std::move(association));
    }

    return result;
}

bool Andromeda::Entanglement::FileAssociationManager::associate(const FileAssociation& fileAssociation) {
    if (!fileAssociation.macDetails)
    {
        return false;
    }

    if (fileAssociation.macDetails->uti.empty())
    {
        return false;
    }

    //NSString* bundleIdentifier = [[NSBundle mainBundle]
    // bundleIdentifier];

NSString* bundleIdentifier =
    [NSString stringWithUTF8String:
        fileAssociation
            .macDetails
            ->bundleIdentifier
            .c_str()];

    NSString* uti =
        [NSString stringWithUTF8String:
            fileAssociation
                .macDetails
                ->uti
                .c_str()];

    LSRolesMask roleMask = MacFileAssociationUtil::macRole2LsRoleMask(fileAssociation.macDetails->role);

    //https://github.com/ghostty-org/ghostty/discussions/8111
    //https://developer.apple.com/documentation/appkit/nsworkspace/setdefaultapplication(at:toopen:completion:)
    OSStatus status =
        LSSetDefaultRoleHandlerForContentType(
            (__bridge CFStringRef)uti,
            roleMask,
            (__bridge CFStringRef)bundleIdentifier);

    return status == noErr;
}