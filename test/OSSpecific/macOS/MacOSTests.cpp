//
// Created by Volkan Orhan on 27.06.2026.
//

#include "MacOSTests.hpp"


void MacOSTests::testAssociationQuery() {
    std::string extension = "mp4";
    //std::string extension = "mod";
    auto editorAssociation = FileAssociationManager::queryAssociation(extension, MacAssociationRole::Editor);
    auto viewerAssociation = FileAssociationManager::queryAssociation(extension, MacAssociationRole::Viewer);
}

void MacOSTests::testAssociationQueryWithoutRole() {
    std::string extension = "png";
    //std::string extension = "mod";
    auto association = FileAssociationManager::queryAssociation(extension);
    return;
}

void MacOSTests::testAssociationRequestForMac() {
    MacFileAssociationRequest associationRequest;
    associationRequest.associationRole= MacAssociationRole::Viewer;
    associationRequest.extension = "mod";
    associationRequest.uti = "com.firecore.fileformat.mod";
    associationRequest.bundleIdentifier = "org.ModPlug.ModPlugPlayer";
    FileAssociationManager::associate(associationRequest);
}

void MacOSTests::testMultipleAssociationQuery() {
    auto associations = FileAssociationManager::queryAssociations({"zip", "mp4", "mod", "foo-bar"});
    auto associations2 = FileAssociationManager::queryAssociations({"mod"});
}
