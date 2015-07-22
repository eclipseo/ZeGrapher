#-------------------------------------------------
#
# Project created by QtCreator 2011-03-25T16:51:48
#
#-------------------------------------------------


QT += widgets printsupport

TARGET = ZeGrapher
TEMPLATE = app
TRANSLATIONS = ZeGrapher_en.ts
CONFIG += separate_debug_info


SOURCES += \
    main.cpp \
    informations.cpp \
    Windows/zegrapher.cpp \
    Windows/fentableauvaleurs.cpp \
    Windows/fenetreoptions.cpp \
    Windows/fenetrefonctions.cpp \
    Windows/fenetrebornes.cpp \
    Windows/apropos.cpp \
    Widgets/tangentwidget.cpp \
    Widgets/straightlinewidget.cpp \
    Widgets/seqwidget.cpp \
    Widgets/qcolorbutton.cpp \
    Widgets/pareqwidget.cpp \
    Widgets/pareqcontroller.cpp \
    Widgets/parconfwidget.cpp \
    Widgets/keyboard.cpp \
    Widgets/integrationwidget.cpp \
    Widgets/funcwidget.cpp \
    Widgets/abstractfuncwidget.cpp \
    ValuesTable/valuestableconf.cpp \
    ValuesTable/valuestable.cpp \
    ValuesTable/seqtable.cpp \
    ValuesTable/pareqtable.cpp \
    ValuesTable/functable.cpp \
    ValuesTable/abstracttable.cpp \
    GraphDraw/printpreview.cpp \
    GraphDraw/maingraph.cpp \
    GraphDraw/imagepreview.cpp \
    GraphDraw/graphdraw.cpp \
    Export/print.cpp \
    Export/imagesave.cpp \
    DataPlot/rowselectorwidget.cpp \
    DataPlot/rowactionswidget.cpp \
    DataPlot/datawindow.cpp \    
    DataPlot/datatable.cpp \
    DataPlot/columnselectorwidget.cpp \
    DataPlot/columnactionswidget.cpp \
    Calculus/treecreator.cpp \
    Calculus/seqcalculator.cpp \
    Calculus/funcvaluessaver.cpp \
    Calculus/funccalculator.cpp \
    Calculus/exprcalculator.cpp \
    Calculus/colorsaver.cpp \
    Widgets/datawidget.cpp \
    DataPlot/csvhandler.cpp \
    Calculus/polynomial.cpp \
    Calculus/polynomialregression.cpp \
    Calculus/regression.cpp \
    Calculus/regressionvaluessaver.cpp \
    DataPlot/modelchoicewidget.cpp \
    DataPlot/modelwidget.cpp \
    DataPlot/polynomialmodelwidget.cpp \
    Widgets/numberlineedit.cpp

HEADERS  += \
    Structures.h \
    informations.h \
    Windows/zegrapher.h \
    Windows/fentableauvaleurs.h \
    Windows/fenetreoptions.h \
    Windows/fenetrefonctions.h \
    Windows/fenetrebornes.h \
    Windows/apropos.h \
    Widgets/tangentwidget.h \
    Widgets/straightlinewidget.h \
    Widgets/seqwidget.h \
    Widgets/qcolorbutton.h \
    Widgets/pareqwidget.h \
    Widgets/pareqcontroller.h \
    Widgets/parconfwidget.h \
    Widgets/keyboard.h \
    Widgets/integrationwidget.h \
    Widgets/funcwidget.h \
    Widgets/abstractfuncwidget.h \
    ValuesTable/valuestableconf.h \
    ValuesTable/valuestable.h \
    ValuesTable/seqtable.h \
    ValuesTable/pareqtable.h \
    ValuesTable/functable.h \
    ValuesTable/abstracttable.h \
    GraphDraw/printpreview.h \
    GraphDraw/maingraph.h \
    GraphDraw/imagepreview.h \
    GraphDraw/graphdraw.h \
    Export/print.h \
    Export/imagesave.h \
    DataPlot/rowselectorwidget.h \
    DataPlot/rowactionswidget.h \
    DataPlot/datawindow.h \   
    DataPlot/datatable.h \
    DataPlot/columnselectorwidget.h \
    DataPlot/columnactionswidget.h \
    Calculus/treecreator.h \
    Calculus/seqcolorssaver.h \
    Calculus/seqcalculator.h \
    Calculus/funcvaluessaver.h \
    Calculus/funccalculator.h \
    Calculus/exprcalculator.h \
    Calculus/colorsaver.h \
    Calculus/calculusdefines.h \
    Widgets/datawidget.h \
    DataPlot/csvhandler.h \
    Calculus/polynomial.h \
    Calculus/polynomialregression.h \
    Calculus/regression.h \
    Calculus/regressionvaluessaver.h \
    DataPlot/modelchoicewidget.h \
    DataPlot/modelwidget.h \
    DataPlot/polynomialmodelwidget.h \
    Widgets/numberlineedit.h


FORMS    += \
    Windows/fenetreoptions.ui \
    Windows/fenetrefonctions.ui \
    Windows/fenetrebornes.ui \
    Windows/apropos.ui \
    Widgets/keyboard.ui \
    Widgets/datawidget.ui \
    Export/print.ui \
    Export/imagesave.ui \
    DataPlot/startingactions.ui \
    DataPlot/sortoptions.ui \
    DataPlot/filloptions.ui \
    DataPlot/datawindow.ui \   
    DataPlot/confirmdelete.ui \
    DataPlot/csvconfig.ui \
    DataPlot/modelchoicewidget.ui \
    DataPlot/polynomialmodelwidget.ui


RESOURCES += \
    ressources.qrc

RC_FILE += \
    ressources.rc

OTHER_FILES += \
    TODO.txt







































































