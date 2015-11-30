/*
 * document.h
 * Copyright 2015, Thorbjørn Lindeijer <bjorn@lindeijer.nl>
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

#ifndef TILED_INTERNAL_DOCUMENT_H
#define TILED_INTERNAL_DOCUMENT_H

#include <QObject>
#include <QString>

class QUndoStack;

namespace Tiled {
namespace Internal {

/**
 * Keeps track of a file and its undo history.
 */
class Document : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(bool modified READ isModified NOTIFY modifiedChanged)

public:
    Document(const QString &fileName = QString(),
             QObject *parent = nullptr);

    QString fileName() const;

    QUndoStack *undoStack() const;
    bool isModified() const;

signals:
    void fileNameChanged(const QString &fileName,
                         const QString &oldFileName);
    void modifiedChanged();

protected:
    void setFileName(const QString &fileName);

    QString mFileName;
    QUndoStack *mUndoStack;
};


inline QString Document::fileName() const
{
    return mFileName;
}

/**
 * Returns the undo stack of this document. Should be used to push any commands
 * on that modify the document.
 */
inline QUndoStack *Document::undoStack() const
{
    return mUndoStack;
}

} // namespace Internal
} // namespace Tiled

#endif // TILED_INTERNAL_DOCUMENT_H
