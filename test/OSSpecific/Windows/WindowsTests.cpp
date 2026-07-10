/*
Windows test definitions of Andromeda Entanglement Project
Copyright (C) 2026 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "WindowsTests.hpp"

#include <iostream>
#include <ostream>
#include <WindowsRegistryUtil.hpp>

#include <EntanglementObjects.hpp>

#include "WindowsShellUtil.hpp"

using namespace Andromeda::Entanglement;
using namespace std;

void printId(optional<string> id, string name) {
    if (id.has_value())
        std::cout << name << " value is " << id.value() << std::endl;
    else
        std::cout<< name <<" has no value" << std::endl;
}

void testProgId(string extension) {
    cout << "Extension: " << extension << endl;
    std::optional<std::string> id = WindowsRegistryUtil::getProgId(extension, WindowsAssociationScope::AllUsers);
    printId(id, "Prog id for all users");
    id = WindowsRegistryUtil::getProgId(extension, WindowsAssociationScope::CurrentUser);
    printId(id, "Prog id for current user");
    id = WindowsRegistryUtil::getUserChoiceProgId(extension);
    printId(id, "User choice prog id");
    cout<<endl;
}

void testCommand() {
    auto progId = WindowsRegistryUtil::getProgId(
    "txt",
    WindowsAssociationScope::AllUsers);

    if (progId)
    {
        auto command = WindowsRegistryUtil::getCommand(
            *progId,
            WindowsAssociationScope::AllUsers);

        if (command)
            std::cout << *command << std::endl;
    }
}

void testDefaultIcon() {
    auto icon = WindowsRegistryUtil::getDefaultIcon("txtfilelegacy", WindowsAssociationScope::AllUsers);
    if(icon)
        std::cout << "Default icon for txt is " << icon->resourcePath << std::endl;
    auto icon2 = WindowsShellUtil::getAssociatedIcon(".txt");
    if(icon2)
    cout<< icon2->resourcePath << std::endl;
}

void WindowsTests::testWindowsRegistryUtil() {
    //static std::vector<std::pair<WindowsAssociationScope, std::string>> ids = WindowsRegistryUtil::getProgIds(".txt");
    testProgId("txt");
    testProgId("mpeg");
    testProgId("mp3");
    testProgId("mp4");
    testProgId("bmp");
    testProgId("xm");
    testProgId("mod");
    testProgId("s3m");
    testCommand();
    testDefaultIcon();
}

