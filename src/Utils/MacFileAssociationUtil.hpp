/*
MacUtils class declarations of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#pragma once
#include <vector>
#include "../EntanglementObjects.hpp"
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Foundation/NSString.h>
#import <CoreServices/CoreServices.h>
#include <string>
#include <optional>

namespace Andromeda::Entanglement {
    class MacFileAssociationUtil {
    public:
        static std::string queryDescription(CFStringRef uti);
        static std::optional<IconInfo> queryDocumentIcon(NSBundle* appBundle, NSString* extension);
        static NSString* currentBundleIdentifier();
        static LSRolesMask macAssociationRoles2LsRoleMask(const MacAssociationRoles &roles);
        static MacAssociationRoles lsRoleMask2macAssociationRoles(const LSRolesMask &lsRoleMask);
        static MacAssociationRole lsRoleMask2macAssociationRole(const LSRolesMask &lsRoleMask);
        static LSRolesMask macAssociationRole2LsRoleMask(const MacAssociationRole &macAssociationRole);
    };
}
