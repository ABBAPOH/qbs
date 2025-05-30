/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qbs.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms and
** conditions see http://www.qt.io/terms-conditions. For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "msbuildtargetproject.h"
#include "msbuildutils.h"
#include "visualstudiogenerator.h"

#include "msbuild/msbuildimport.h"
#include "msbuild/msbuildimportgroup.h"
#include "msbuild/msbuilditem.h"
#include "msbuild/msbuilditemgroup.h"
#include "msbuild/msbuildproperty.h"
#include "msbuild/msbuildpropertygroup.h"

namespace qbs {

class MSBuildTargetProjectPrivate
{
public:
    MSBuildTargetProjectPrivate(const Internal::VisualStudioVersionInfo &versionInfo)
        : versionInfo(versionInfo) {}
    MSBuildPropertyGroup *globalsPropertyGroup = nullptr;
    MSBuildProperty *projectGuidProperty = nullptr;
    const Internal::VisualStudioVersionInfo &versionInfo;
};

MSBuildTargetProject::MSBuildTargetProject(
    const GeneratableProject &project, const Internal::VisualStudioVersionInfo &versionInfo)
    : d(new MSBuildTargetProjectPrivate(versionInfo))
{
    setDefaultTargets(QStringLiteral("Build"));
    setToolsVersion(versionInfo.toolsVersion());

    const auto projectConfigurationsGroup = makeChild<MSBuildItemGroup>();
    projectConfigurationsGroup->setLabel(QStringLiteral("ProjectConfigurations"));

    for (auto it = project.projects.cbegin(), end = project.projects.cend(); it != end; ++it) {
        const auto item = projectConfigurationsGroup->makeChild<MSBuildItem>(
            QStringLiteral("ProjectConfiguration"));
        item->setInclude(MSBuildUtils::fullName(it.value()));
        item->appendProperty(QStringLiteral("Configuration"), it.key());
        item->appendProperty(QStringLiteral("Platform"), MSBuildUtils::platform(it.value()));
    }

    d->globalsPropertyGroup = makeChild<MSBuildPropertyGroup>();
    d->globalsPropertyGroup->setLabel(QStringLiteral("Globals"));
    d->projectGuidProperty = d->globalsPropertyGroup->makeChild<MSBuildProperty>(
        QStringLiteral("ProjectGuid"), QUuid::createUuid().toString());

    // Trigger creation of the property sheets ImportGroup
    propertySheetsImportGroup();
}

MSBuildTargetProject::~MSBuildTargetProject() = default;

const Internal::VisualStudioVersionInfo &MSBuildTargetProject::versionInfo() const
{
    return d->versionInfo;
}

QUuid MSBuildTargetProject::guid() const
{
    return QUuid{d->projectGuidProperty->value().toString()};
}

void MSBuildTargetProject::setGuid(const QUuid &guid)
{
    d->projectGuidProperty->setValue(guid.toString());
}

MSBuildPropertyGroup *MSBuildTargetProject::globalsPropertyGroup()
{
    return d->globalsPropertyGroup;
}

MSBuildImportGroup *MSBuildTargetProject::propertySheetsImportGroup()
{
    MSBuildImportGroup *importGroup = nullptr;
    for (const auto &child : children()) {
        if (auto group = dynamic_cast<MSBuildImportGroup *>(child.get())) {
            if (group->label() == QStringLiteral("PropertySheets")) {
                importGroup = group;
                break;
            }
        }
    }

    if (!importGroup) {
        importGroup = makeChild<MSBuildImportGroup>();
        importGroup->setLabel(QStringLiteral("PropertySheets"));
    }

    return importGroup;
}

void MSBuildTargetProject::appendPropertySheet(const QString &path, bool optional)
{
    const auto import = propertySheetsImportGroup()->makeChild<MSBuildImport>();
    import->setProject(path);
    if (optional)
        import->setCondition(QStringLiteral("Exists('%1')").arg(path));
}

} // namespace qbs
