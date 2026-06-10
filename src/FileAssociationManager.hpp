//
// Created by Volkan Orhan on 9.06.2026.
//
#pragma once
#include <vector>
#include "EntanglementObjects.hpp"

namespace Andromeda::Entanglement {
    class FileAssociationManager {
    public:
        std::vector<FileAssociation> queryAssociations(const std::vector<std::string>& extensions);

        bool associate(const FileAssociation& fileAssociation);

        bool unassociate(const std::string& extension);
    };
}
