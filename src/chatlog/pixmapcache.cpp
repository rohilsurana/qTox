/*
    Copyright (C) 2015 by Project Tox <https://tox.im>

    This file is part of qTox, a Qt-based graphical interface for Tox.

    This program is libre software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    See the COPYING file for more details.
*/

#include "pixmapcache.h"

QPixmap PixmapCache::get(const QString &filename, QSize size)
{
    auto itr = cache.find(filename);

    if (itr == cache.end())
    {
        QIcon icon;
        icon.addFile(filename);

        cache.insert(filename, icon);
        return icon.pixmap(size);
    }

    return itr.value().pixmap(size);
}

PixmapCache &PixmapCache::getInstance()
{
    static PixmapCache instance;
    return instance;
}

