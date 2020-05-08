/***************************************************************************
 * Copyright (c) 2007-2020                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase project, a images/documents           *
 * database program.                                                       *
 *                                                                         *
 * Redistribution and use in source and binary forms, with or without      *
 * modification, are permitted provided that the following conditions      *
 * are met:                                                                *
 * 1. Redistributions of source code must retain the above copyright       *
 *    notice, this list of conditions and the following disclaimer.        *
 * 2. Redistributions in binary form must reproduce the above copyright    *
 *    notice, this list of conditions and the following disclaimer in the  *
 *    documentation and/or other materials provided with the distribution. *
 * 4. Neither the name of the Author     nor the names of its contributors *
 *    may be used to endorse or promote products derived from this software*
 *    without specific prior written permission.                           *
 *                                                                         *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      *
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS  *
 * BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR   *
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF    *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS*
 * INTERRUPTION)                                                           *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,     *
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING   *
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE      *
 * POSSIBILITY OFSUCH DAMAGE.                                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "typeimage.h"

TypeImage::TypeImage()
{
    initMaps();

}

void TypeImage::initMaps(void)
{
    mapTypes["txt"] = "txt-109.png";
    mapTypes["asc"] = "asc-4704.png";
    mapTypes["avi"] = "avi-10.png";
    mapTypes["dvi"] = "div-568.png";
    mapTypes["cdr"] = "cdr-329.png";

    mapTypes["doc"] = "doc.png";
    mapTypes["docx"] = "docx.png";
    mapTypes["dotx"] = "dotx-1756.png";
    mapTypes["xls"] = "xls-121.png";
    mapTypes["xlsx"] = "xlsx.png";

    mapTypes["pdf"] = "pdf.png";
    mapTypes["dwg"] = "dwg-256.png";
    mapTypes["dws"] = "dws-5288.png";
    mapTypes["7z"] = "7z-452.png";
    mapTypes["zipx"] = "7z-452.png";

    mapTypes["arj"] = "arj-233.png";
    mapTypes["zip"] = "zip-128.png";
    mapTypes["exe"] = "exe-29.png";
    mapTypes["pps"] = "pps-80.png";
    mapTypes["ppt"] = "ppt-81.png";

    mapTypes["odt"] = "odt-716.png";
    mapTypes["odf"] = "odt-716.png";
    mapTypes["ods"] = "ods-1215.png";
    mapTypes["odb"] = "odb-4855.png";
    mapTypes["mp3"] = "mp3-58.png";
    mapTypes["mpa"] = "mpa-625.png";

    mapTypes["jpeg"] = "jpeg.png";
    mapTypes["jpg"] = "jpeg.png";

    mapTypes["mpeg"] = "mpa-625.png";
    mapTypes["mpg"] = "mpa-625.png";
    mapTypes["ogg"] = "ogg-413.png";
    mapTypes["ra"] = "ra-89.png";
    mapTypes["wav"] = "wav-113.png";
    mapTypes["wma"] = "wma-115.png";

    mapTypes["pct"] = "pct-73.png";
    mapTypes["db"] = "db-20.png";
    mapTypes["odf"] = "odf-560.png";
    mapTypes["mdf"] = "mdf-1372.png";
    mapTypes["asp"] = "asp-8.png";
    mapTypes["aspx"] = "aspx-9.png";

    mapTypes["html"] = "pagehtml.png";
    mapTypes["htm"] = "pagehtml.png";

}

/**
 * @brief TypeImage::resourceName
 *        Retorna o nome do resouce basado na extensao
 *        do nome do arquivo informado
 * @param type - Informar somente a extensao ex: doc, xls ...
 * @return
 */
QString TypeImage::resourceName(const QString type)
{
    QString s;
    if(mapTypes.contains( type ) ) {
        s = QString(":/%0").arg(mapTypes.value(type));
    } else {
        s = ":/generic.png";
    }
    return s;
}

/**
 * @brief TypeImage::type
 *        Retorna o nome do resouce basado na extensao
 *        do nome do arquivo informado
 * @param fileName - nome completo do arquivo: ex. arquivo.doc
 * @return QPixmap
 */
QPixmap TypeImage::type(const QString fileName)
{
    QString s;
    QFileInfo fi(fileName);

    if(mapTypes.contains(fi.suffix().toLower())) {
        s = QString(":/%0").arg(mapTypes.value(fi.suffix().toLower()));
    } else {
        s = "";
    }

    QPixmap pix;
    if(!s.isEmpty())
        pix.load(s);
    else
        pix.load(":/generic.png");

    return pix;
}

/**
 * @brief TypeImage::isImageViewable
 *        Verifica se um arquivo de imagem visualizavel
 *        dentro do sistema
 * @param fileName
 * @return true | false
 */
bool TypeImage::isImageViewable(const QString fileName)
{
    if(fileName.isEmpty())
        return false;

    QFileInfo fi(fileName);

    return stringListViewable().contains(fi.suffix().toLower());
}

/**
 * @brief TypeImage::stringListViewable
 *        Retorna um tipo QStringList() com as extensoes
 *        dos tipos visiveis
 *        Isso e usado em diversas partes do progama para
 *        diversos propositos
 *
 *        C++11 compliant - Inicializacao da lista
 * @return
 */
QStringList TypeImage::stringListViewable()
{
    /**
     * BMP	Windows Bitmap	Read/write
     * GIF	Graphic Interchange Format (optional)	Read
     * JPG	Joint Photographic Experts Group	Read/write
     * JPEG	Joint Photographic Experts Group	Read/write
     * PNG	Portable Network Graphics	Read/write
     * PBM	Portable Bitmap	Read
     * PGM	Portable Graymap	Read
     * PPM	Portable Pixmap	Read/write
     * XBM	X11 Bitmap	Read/write
     * XPM	X11 Pixmap	Read/write
     */
    QStringList list = {
        "bmp", "jpg", "jpeg", "png",
         "pbm","pgm", "ppm", "xbm",
         "xpm"
    };
    return list;
}
