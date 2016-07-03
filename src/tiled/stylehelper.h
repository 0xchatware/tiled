/*
 * stylehelper.h
 * Copyright 2016, Thorbjørn Lindeijer <bjorn@lindeijer.nl>
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

#ifndef TILED_INTERNAL_STYLEHELPER_H
#define TILED_INTERNAL_STYLEHELPER_H

#include <QPalette>
#include <QString>

namespace Tiled {
namespace Internal {

class StyleHelper
{
public:
    static void initialize();

    static const QString &defaultStyle() { return mDefaultStyle; }
    static const QPalette &defaultPalette() { return mDefaultPalette; }

private:
    static QString mDefaultStyle;
    static QPalette mDefaultPalette;
};

} // namespace Internal
} // namespace Tiled

#endif // TILED_INTERNAL_STYLEHELPER_H
