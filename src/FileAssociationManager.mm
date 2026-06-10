#include <vector>
#include <string>
#include <memory>

#include "FileAssociationManager.hpp"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <CoreServices/CoreServices.h>

std::vector<Andromeda::Entanglement::FileAssociation>
Andromeda::Entanglement::FileAssociationManager::queryAssociations(
    const std::vector<std::string>& extensions)
{
    std::vector<Andromeda::Entanglement::FileAssociation> result;

    NSString* currentBundleId =
        [[NSBundle mainBundle] bundleIdentifier];

    for (const auto& ext : extensions)
    {
        Andromeda::Entanglement::FileAssociation association;

        association.fileTypeInfo.extension = ext;

        NSString* nsExt =
            [NSString stringWithUTF8String:ext.c_str()];

        CFStringRef uti =
            UTTypeCreatePreferredIdentifierForTag(
                kUTTagClassFilenameExtension,
                (__bridge CFStringRef)nsExt,
                nullptr);

        if (uti)
        {
            association.macDetails.emplace();

            association.macDetails->uti =
                [(NSString*)uti UTF8String];

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

                association.handledByCurrentApplication =
                    [handlerBundleId isEqualToString:
                        currentBundleId];

                NSWorkspace* workspace =
                    [NSWorkspace sharedWorkspace];

                NSURL* appUrl =
                    [workspace
                        URLForApplicationWithBundleIdentifier:
                            handlerBundleId];

                if (appUrl)
                {
                    NSBundle* appBundle =
                        [NSBundle bundleWithURL:appUrl];

                    NSString* appName =
                        [appBundle
                            objectForInfoDictionaryKey:
                                @"CFBundleName"];

                    if (appName)
                    {
                        association.applicationInfo.applicationName =
                            [appName UTF8String];
                    }
                }

                CFRelease(handler);
            }

            CFRelease(uti);
        }

        result.push_back(std::move(association));
    }

    return result;
}