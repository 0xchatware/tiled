/*
 * templatesdock.cpp
 * Copyright 2017, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright 2017, Mohamed Thabet <thabetx@gmail.com>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "templatesdock.h"

#include "objecttemplatemodel.h"
#include "preferences.h"
#include "tmxmapformat.h"
#include "utils.h"

#include <QBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolBar>

using namespace Tiled;
using namespace Tiled::Internal;

TemplatesDock::TemplatesDock(QWidget *parent):
    QDockWidget(parent),
    mTemplatesView(new TemplatesView),
    mNewTemplateGroup(new QAction(this))
{
    setObjectName(QLatin1String("TemplatesDock"));

    QWidget *widget = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(mTemplatesView);

    mNewTemplateGroup->setIcon(QIcon(QLatin1String(":/images/16x16/document-new.png")));
    Utils::setThemeIcon(mNewTemplateGroup, "document-new");
    connect(mNewTemplateGroup, SIGNAL(triggered()), SLOT(newTemplateGroup()));

    QToolBar *toolBar = new QToolBar;
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setIconSize(Utils::smallIconSize());

    toolBar->addAction(mNewTemplateGroup);

    layout->addWidget(toolBar);
    setWidget(widget);
    retranslateUi();
}

void TemplatesDock::newTemplateGroup()
{
    QString filter = TtxTemplateGroupFormat().nameFilter();

    Preferences *prefs = Preferences::instance();
    QString suggestedFileName = prefs->lastPath(Preferences::TemplateDocumentsFile);
    suggestedFileName += QLatin1String("/untitled.ttx");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    suggestedFileName,
                                                    filter);

    if (fileName.isEmpty())
        return;

    QString error;
    if (!mTemplatesView->objectTemplateModel()->addNewDocument(fileName, &error)) {
        QMessageBox::critical(this, tr("Error Creating Template Group"), error);
        return;
    }

    prefs->setLastPath(Preferences::TemplateDocumentsFile,
                       QFileInfo(fileName).path());
}

void TemplatesDock::retranslateUi()
{
    setWindowTitle(tr("Templates"));
    mNewTemplateGroup->setText(tr("New Template Group"));
}

TemplatesView::TemplatesView(QWidget *parent)
    : QTreeView(parent),
      mObjectTemplateModel(new ObjectTemplateModel)
{
    setUniformRowHeights(true);
    setHeaderHidden(true);

    Preferences *prefs = Preferences::instance();
    mObjectTemplateModel->setTemplateDocuments(prefs->templateDocuments());

    setModel(mObjectTemplateModel);

    setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(mObjectTemplateModel, &ObjectTemplateModel::dataChanged,
            this, &TemplatesView::applyTemplateGroups);
    connect(mObjectTemplateModel, &ObjectTemplateModel::rowsInserted,
            this, &TemplatesView::applyTemplateGroups);
}

void TemplatesView::applyTemplateGroups()
{
    auto templateGroups = mObjectTemplateModel->templateDocuments();
    Preferences *prefs = Preferences::instance();
    prefs->setTemplateDocuments(templateGroups);

    QString templateDocumentsFile = prefs->templateDocumentsFile();
    QDir templateDocumentsDir = QFileInfo(templateDocumentsFile).dir();

    if (!templateDocumentsDir.exists())
        templateDocumentsDir.mkpath(QLatin1String("."));

    TemplateDocumentsSerializer serializer;
    if (!serializer.writeTemplateDocuments(templateDocumentsFile, templateGroups)) {
        QMessageBox::critical(this, tr("Error Writing Template Groups"),
                              tr("Error writing to %1:\n%2")
                              .arg(prefs->templateDocumentsFile(),
                                   serializer.errorString()));
    }
}

QSize TemplatesView::sizeHint() const
{
    return Utils::dpiScaled(QSize(130, 100));
}
