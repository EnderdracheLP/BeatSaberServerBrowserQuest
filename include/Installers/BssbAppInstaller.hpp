#pragma once
#include "custom-types/shared/macros.hpp"

#include "Zenject/Installer.hpp"

DECLARE_CLASS_CODEGEN(ServerBrowser::Installers, BssbAppInstaller, ::Zenject::Installer) {
    DECLARE_OVERRIDE_METHOD_MATCH(void, InstallBindings, &::Zenject::Installer::InstallBindings);
    DECLARE_DEFAULT_CTOR();
};
