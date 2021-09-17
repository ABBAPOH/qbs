Qbs 1.20 Released
=================

The [Qbs build tool](http://qbs.io) version 1.20.0 is available together with a
[Visual Studio Code](https://www.qt.io/blog/qbs-1.19.0-released-0) extension v1.0.5.

Qbs is a community-driven language-agnostic build automation system. It is fast
and offers an easy-to-learn language based upon QML.

What's new
----------

About 107 contributions went into this release since version 1.19.0. We have
selected a few items below. Have a look into the complete
[changelog](https://code.qt.io/cgit/qbs/qbs.git/tree/changelogs/changes-1.20.0.md)
if you are interested in more details.

### General

- We finished porting Qbs to Qt6. Both CMake and Qbs builds now fully support building with Qt6.
  QtScript module, which is not shipped with Qt6, has been updated to the recent dev branch
  and ported to the recent C++ standard. When building with Qt6, Qbs uses bundled QtScript
  module added as a Git submodule.
- When invoking `qbs build` with the different set of properties compared to the previous
  invocation, Qbs prints the old set of properties.
- Added convenience command to qbs-config to add a profile in one go instead of
  setting properties separately. This dramatically improves QtCreator's performance on startup
  when multiple Android Kits are present
  ([QTCREATORBUG-25463](https://bugreports.qt.io/browse/QTCREATORBUG-25463)).
- We've added a profiling timer for [module providers](https://doc.qt.io/qbs/module-providers.html).
  It can be useful in the future when more module providers providers will be added.
- macOS [Homebrew](https://formulae.brew.sh/formula/qbs) formula has been ported from
  QMake to CMake. We plan to deprecate the QMake build some day in the future.

### C/C++ Support

- Added support for a bunch of COSMIC compilers - COLDFIRE (also known as M68K), HCS08,
  HCS12, STM8 and STM32 compilers.
- Added support for the new [Digital Mars](https://www.digitalmars.com/) toolchain
  ([QBS-1636](https://bugreports.qt.io/browse/QBS-1636)).
- The new [cpp.enableCxxLanguageMacro](https://doc.qt.io/qbs/qml-qbsmodules-cpp.html#enableCxxLanguageMacro-prop)
  property was added for the MSVC toolchain that controls the /Zc:__cplusplus flag required for
  proper support of the new C++ standards ([QBS-1655](https://bugreports.qt.io/browse/QBS-1655)).
- Added support for the "c++20" language version for the MSVC toolchain.
  which results in adding the /std:c++latest flag ([QBS-1656](https://bugreports.qt.io/browse/QBS-1656)).
- Fixed handling of the cpp.linkerWrapper property with the MSVC toolchain
  ([QBS-1653](https://bugreports.qt.io/browse/QBS-1653)).

### Qt Support

- We fixed support for qml binaries that were moved to the /libexec directory in Qt 6.2
  ([QBS-1636](https://bugreports.qt.io/browse/QBS-1636)).

### Android Support

- It is now possible to use dex compiler d8 instead of dx via the
  Android.sdk.dexCompilerName property.
- We removed Ministro support since the latest Qt version that supports it is Qt 5.7
- Fixed linking with static stl on Android ([QBS-1654](https://bugreports.qt.io/browse/QBS-1654)).
- The default Android Asset Packaging Tool was changed from aapt to aapt2 - aapt2 was introduced
  in Build Tools since 26.0.2 and replaced aapt in gradle since version 3.0.0.
  Since Android.sdk now requires Build Tool version 24.0.3, QBS now defaults to aapt2.

Try it
------

Qbs is available for download on the [download
page](https://download.qt.io/official_releases/qbs/1.20.0). Please
report issues in our [bug tracker](https://bugreports.qt.io/browse/QBS/). Join our [Discord
server](https://discord.gg/tw5HHyY) for live discussions. You can use our
[mailing list](https://lists.qt-project.org/mailman/listinfo/qbs) for questions
and discussions. The [documentation](https://doc.qt.io/qbs/index.html)
and [wiki](https://wiki.qt.io/Qbs) are also good places to get started.

Qbs is also available from a number of package repositories
([Chocolatey](https://chocolatey.org/packages/qbs),
[MacPorts](https://www.macports.org/ports.php?by=name&substr=qbs),
[Homebrew](https://formulae.brew.sh/formula/qbs)) and is updated on each
release by the Qbs development team. It can also be installed through
the native package management system on a number of Linux distributions.
Please find a complete overview on
[repology.org](https://repology.org/project/qbs/versions).

Contribute
----------

If You are a happy user of Qbs, please tell others about it. But maybe you would
like to contribute something. Everything that makes Qbs better is highly
appreciated. Contributions may consist of reporting bugs or fixing them right
away. But also new features are very welcome. Your patches will be automatically
sanity-checked, built and verified on Linux, macOS and Windows by our CI bot.

Get started with instructions in the [Qbs Wiki](https://wiki.qt.io/Qbs).

Thanks to everybody who made the 1.20.0 release happen:

* Christian Kandeler
* Denis Shienkov
* Eike Ziller
* Ivan Komissarov
* Jan Blackquill
* Mitch Curtis
* Oswald Buddenhagen
* Raphaël Cotty
* Richard Weickelt
