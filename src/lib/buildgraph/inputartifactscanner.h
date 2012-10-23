/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Build Suite.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef INPUTARTIFACTSCANNER_H
#define INPUTARTIFACTSCANNER_H

#include "scanresultcache.h"

#include <QHash>
#include <QStringList>
#include <QSharedPointer>

struct ScannerPlugin;

namespace qbs {

class Artifact;
class PropertyMap;

struct ResolvedDependency
{
    ResolvedDependency()
        : artifact(0)
    {}

    bool isValid() const { return !filePath.isNull(); }

    QString filePath;
    Artifact *artifact;
};

class InputArtifactScannerContext
{
public:
    InputArtifactScannerContext(ScanResultCache *scanResultCache);
    ~InputArtifactScannerContext();

private:
    ScanResultCache *scanResultCache;

    struct ResolvedDependencyCacheItem
    {
        ResolvedDependencyCacheItem()
            : valid(false)
        {}

        bool valid;
        ResolvedDependency resolvedDependency;
    };

    typedef QHash<QString, QHash<QString, ResolvedDependencyCacheItem> > ResolvedDependenciesCache;

    struct CacheItem
    {
        CacheItem()
            : valid(false)
        {}

        bool valid;
        QStringList includePaths;
        ResolvedDependenciesCache resolvedDependenciesCache;
    };

    QHash<QSharedPointer<const PropertyMap>, CacheItem> cache;

    friend class InputArtifactScanner;
};

class InputArtifactScanner
{
public:
    InputArtifactScanner(Artifact *artifact, InputArtifactScannerContext *ctx);
    void scan();
    bool newDependencyAdded() const { return m_newDependencyAdded; }

private:
    void scanForFileDependencies(ScannerPlugin *scannerPlugin, const QStringList &includePaths,
            Artifact *inputArtifact, InputArtifactScannerContext::ResolvedDependenciesCache &cacheItem);
    void resolveScanResultDependencies(const QStringList &includePaths,
            const Artifact *inputArtifact, const ScanResultCache::Result &scanResult,
            const QString &filePathToBeScanned, QStringList *filePathsToScan, InputArtifactScannerContext::ResolvedDependenciesCache &cacheItem);
    void handleDependency(ResolvedDependency &dependency);

    Artifact * const m_artifact;
    InputArtifactScannerContext *const m_context;
    bool m_newDependencyAdded;
};

} // namespace qbs

#endif // INPUTARTIFACTSCANNER_H
