add_qbs_test(language
    DEFINES
        "QBS_VERSION=\"${QBS_VERSION}\""
    DEPENDS
        qbsscriptengine
    SOURCES
        tst_language.cpp
        tst_language.h
    )
