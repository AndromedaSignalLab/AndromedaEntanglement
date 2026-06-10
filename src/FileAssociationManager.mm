#include <vector>
#include <string>
#include <optional>

#include "FileAssociationManager.hpp"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <CoreServices/CoreServices.h>

static std::optional<Andromeda::Entanglement::IconInfo>
queryDocumentIcon(
    NSBundle* appBundle,
    NSString* extension)
{
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

std::vector<Andromeda::Entanglement::FileAssociation>
Andromeda::Entanglement::FileAssociationManager::queryAssociations(
    const std::vector<std::string>& extensions)
{
    std::vector<Andromeda::Entanglement::FileAssociation> result;

    NSString* currentBundleId =
        [[NSBundle mainBundle] bundleIdentifier];

    NSWorkspace* workspace =
        [NSWorkspace sharedWorkspace];

    for (const auto& ext : extensions)
    {
        Andromeda::Entanglement::FileAssociation association;

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
        // Description (temporary)
        //
        association.fileTypeInfo.description =
            [(NSString*)uti UTF8String];

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
                Andromeda::Entanglement::MacRole::All;

            association.handledByCurrentApplication =
                [handlerBundleId
                    isEqualToString:
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
                // Try document icon first
                //
                auto documentIcon =
                    queryDocumentIcon(
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
                    // Fallback icon
                    //
                    NSImage* icon =
                        [workspace
                            iconForFileType:
                                nsExt];

                    if (icon)
                    {
                        association.fileTypeInfo.iconInfo.iconSource =
                            Andromeda::Entanglement::IconSource::BinaryData;

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