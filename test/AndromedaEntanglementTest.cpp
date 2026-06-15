/*
Test function definitions of Andromeda Entangclement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <iostream>
#include "../src/AndromedaEntanglement.hpp"
#include "../src/FileAssociationManager.hpp"

using namespace Andromeda::Entanglement;

void testMacAssociationRole() {
    std::string extension = "mp4";
    //std::string extension = "mod";
    auto editor = FileAssociationManager::queryAssociation(extension, MacAssociationRole::Editor);
    auto viewer = FileAssociationManager::queryAssociation(extension, MacAssociationRole::Viewer);

}

void testAssociationRequestForMac() {
    MacFileAssociationRequest associationRequest;
    associationRequest.associationRole= MacAssociationRole::Viewer;
    associationRequest.extension = "mod";
    associationRequest.uti = "com.firecore.fileformat.mod";
    associationRequest.bundleIdentifier = "org.ModPlug.ModPlugPlayer";
    FileAssociationManager::associate(associationRequest);
}

void testMultipleExtensionQuery() {
    auto associations = FileAssociationManager::queryAssociations({"zip", "mp4", "mod", "foo-bar"});
    auto associations2 = FileAssociationManager::queryAssociations({"mod"});
}

int main() {
    std::cout << "AndromedaEntanglementTest" << std::endl;

    //testMacAssociationRole();
    //testAssociationRequestForMac();
    testMultipleExtensionQuery();

    return 0;
}
