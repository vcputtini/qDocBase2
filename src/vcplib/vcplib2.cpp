#include "vcplib2.h"


VCPLib2::VCPLib2()
{
}


/**
 * Retorna o nome da organizacao para para ser usado
 * pela classe QSettings
 * Este nome e um UUID gerado pela QUuid()
 * @return nome da organizacao
 */
QString VCPLib2::organization() const
{
  return("015261e5-bded-4c6b-8142-a07be301ba20");
}

/**
 * Retorna a data de hoje tanto no formato ISO, como no formato Europeu
 * ou no formato INTERNO (ddmmaaaa) ou no formato de timestamp do MySQL
 * @param df - DateFormats::Iso ou DateFormats::Eur ou DateFormats::Internal
 *             DateFormats::SqlTimeStamp
 * @return - a data de hoje no formato espeficicado
 */
QString VCPLib2::today(DateFormats df)
{
   QDate date;

   switch(df) {
    case Iso:
        return date.currentDate().toString("yyyy-MM-dd");
        break;
    case Eur:
        return date.currentDate().toString("dd/MM/yyyy");
        break;
    case Internal:
        return date.currentDate().toString("ddMMyyyy");
        break;
    case SqlTimeStamp:
       return date.currentDate().toString("yyyy-MM-dd hh:mm:ss");
   }

   return QString();
}

/**
 * @brief VCPLib2::toISO
 * @param s - data no formato dd/mm/yyyy
 * @return data formato ISO
 */
QString VCPLib2::toISO(const QString s)
{
    QDate date( s.right(4).toInt(),
                s.mid(3,2).toInt(),
                s.left(2).toInt());

    return date.toString("yyyy-MM-dd");
}

/**
 * @brief VCPLib2::toEUR
 * @param s - data no formto yyyy-mm-dd
 * @return data formato EUR
 */
QString VCPLib2::toEUR(const QString s)
{
    QDate date( s.left(4).toInt(),
                s.mid(5,2).toInt(),
                s.right(2).toInt());
    return date.toString("dd/MM/yyyy");
}
