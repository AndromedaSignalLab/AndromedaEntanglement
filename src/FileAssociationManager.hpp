/*
FileAssociationManager class declarations of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#pragma once
#include <vector>
#include "EntanglementObjects.hpp"

namespace Andromeda::Entanglement {
    class FileAssociationManager {
    public:
        static std::vector<FileAssociation> queryAssociations(const std::vector<std::string>& extensions);

        static std::vector<FileAssociation> queryAssociations(const std::vector<std::string>& extensions, const MacAssociationRole &macAssociationRole);

        static FileAssociation queryAssociation(const std::string& extension, const MacAssociationRole &macAssociationRole);

        static std::vector<FileAssociation> queryAssociation(const std::string& extension);

        static bool associate(const FileAssociation& fileAssociation);

        static bool associate(const MacFileAssociationRequest& fileAssociationRequest);

        static bool unassociate(const std::string& extension);
    };
}
