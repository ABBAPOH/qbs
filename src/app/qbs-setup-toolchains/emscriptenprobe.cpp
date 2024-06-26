/****************************************************************************
**
** Copyright (C) 2023 Danya Patrushev <danyapat@yandex.ru>
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qbs.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "emscriptenprobe.h"

#include "../shared/logging/consolelogger.h"
#include "probe.h"

#include <logging/translator.h>
#include <tools/error.h>
#include <tools/hostosinfo.h>
#include <tools/profile.h>
#include <tools/settings.h>

#include <QDir>
#include <QFileInfo>
#include <QProcess>

using namespace qbs;
using Internal::HostOsInfo;
using Internal::Tr;

namespace {

using EmsdkPaths = QVector<QPair<QString, QString>>;

qbs::Profile writeProfile(
    const QString &profileName, const EmsdkPaths &paths, qbs::Settings *settings)
{
    qbs::Profile profile(profileName, settings);
    profile.setValue(QStringLiteral("qbs.architecture"), QStringLiteral("wasm"));
    profile.setValue(QStringLiteral("qbs.toolchainType"), QStringLiteral("emscripten"));
    profile.setValue(QStringLiteral("qbs.targetPlatform"), QStringLiteral("wasm-emscripten"));

    for (const auto &[key, path] : paths)
        profile.setValue(QStringLiteral("cpp.") + key, QDir::fromNativeSeparators(path));

    return profile;
}

EmsdkPaths readEnvironment()
{
    const QString emsdkPath = QString::fromLocal8Bit(qgetenv("EMSDK"));
    if (emsdkPath.isEmpty())
        return {};

    //in case any of these are missing, emcc.qbs validation will fail
    return EmsdkPaths{
        {QStringLiteral("emsdkDir"), emsdkPath},
        {QStringLiteral("emsdkNode"), QString::fromLocal8Bit(qgetenv("EMSDK_NODE"))},
        {QStringLiteral("emsdkPython"), QString::fromLocal8Bit(qgetenv("EMSDK_PYTHON"))},
        {QStringLiteral("javaHome"), QString::fromLocal8Bit(qgetenv("JAVA_HOME"))}};
}

const QString emcc = HostOsInfo::isWindowsHost() ? QStringLiteral("emcc.bat")
                                                 : QStringLiteral("emcc");
const QString emplusplus = HostOsInfo::isWindowsHost() ? QStringLiteral("em++.bat")
                                                       : QStringLiteral("em++");
const QString emsdkEnv = HostOsInfo::isWindowsHost() ? QStringLiteral("emsdk_env.bat")
                                                     : QStringLiteral("emsdk_env.sh");

EmsdkPaths runEmsdkEnv(const QFileInfo &compiler)
{
    //Since we are here, the compiler name should be */emsdk/upstream/emscripten/em[cc|++]
    //(ending with '.bat' for windows)
    QDir emsdkDir = compiler.dir(); // */emsdk/upstream/emscripten
    emsdkDir.cdUp();                // */emsdk/upstream
    emsdkDir.cdUp();                // */emsdk/

    QFileInfo emsdkEnvInfo(emsdkDir.absoluteFilePath(emsdkEnv));
    if (!emsdkEnvInfo.exists())
        throw qbs::ErrorInfo(Tr::tr("%1 does not exist").arg(emsdkEnvInfo.absoluteFilePath()));

    QProcess proc;
    proc.setReadChannel(QProcess::StandardError); //emsdk_env outputs to stderr
    if (HostOsInfo::isWindowsHost()) {
        proc.start(
            QString::fromLocal8Bit(qgetenv("COMSPEC")),
            {QLatin1String("/c"), emsdkEnvInfo.absoluteFilePath()});
    } else {
        proc.start(QString::fromLocal8Bit(qgetenv("SHELL")), {emsdkEnvInfo.absoluteFilePath()});
    }

    if (!proc.waitForFinished(-1) || proc.exitStatus() != QProcess::NormalExit) {
        throw qbs::ErrorInfo(Tr::tr("Failure to run %1: %2")
                                 .arg(emsdkEnvInfo.absoluteFilePath(), proc.errorString()));
    }

    EmsdkPaths paths;
    while (!proc.atEnd()) {
        const char equalStr[] = " = ";
        const QString line = QLatin1String(proc.readLine());
        const int index = line.indexOf(QLatin1String(equalStr));
        if (index == -1)
            continue;

        const QString key = line.left(index);
        if (key == QLatin1String("PATH"))
            continue;
        const QString value = line.mid(index + sizeof(equalStr) - 1);
        paths << qMakePair(key, value.trimmed());
    }
    return paths;
}

EmsdkPaths getEmsdkPaths(const QFileInfo &compiler)
{
    // If the user has already run emsdk_env.bat, all subsequent runs in the same console
    // will return a concise message about "Setting up EMSDK environment" without  all
    // the data that we need, so we we read the environment first.
    const auto environmentPaths = readEnvironment();
    return !environmentPaths.isEmpty() ? environmentPaths : runEmsdkEnv(compiler);
}

} //namespace

bool isEmscriptenCompiler(const QString &compilerName)
{
    return compilerName.startsWith(QLatin1String("emcc"))
           || compilerName.startsWith(QLatin1String("em++"));
}

qbs::Profile createEmscriptenProfile(
    const QFileInfo &compiler, qbs::Settings *settings, const QString &profileName)
{
    qbs::Profile profile = writeProfile(profileName, getEmsdkPaths(compiler), settings);

    qbsInfo() << Tr::tr("Profile '%1' created for '%2'.")
                     .arg(profile.name(), compiler.absoluteFilePath());
    return profile;
}

void emscriptenProbe(qbs::Settings *settings, std::vector<qbs::Profile> &profiles)
{
    qbsInfo() << Tr::tr("Trying to detect emscripten toolchain...");

    const QString compilerPath = findExecutable(emcc);

    if (compilerPath.isEmpty()) {
        qbsInfo() << Tr::tr("No emscripten toolchain found.");
        return;
    }

    const qbs::Profile profile = createEmscriptenProfile(
        QFileInfo(compilerPath), settings, QLatin1String("webassembly"));
    profiles.push_back(profile);
}
