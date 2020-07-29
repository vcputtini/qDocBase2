#include "vcpmdiarea.h"

// ***************************************************************************
// vcpMdiarea -- Cria e mantem uma interface MDI padrao para aplicacoes com
//               Logo grafico
// ***************************************************************************

vcpMdiArea::vcpMdiArea(QWidget *parent, QString nomeProg)
    : QMdiArea(parent),
      NomeProg(nomeProg)
{
       setBackground(Qt::white);
}


void vcpMdiArea::paintEvent( QPaintEvent * event)
{
   QMdiArea::paintEvent(event);
   QPainter painter(viewport());

   // NOTE: O LOGOTIPO ABAIXO DEVE SER COMPILADO JUNTAMENTE
   //      COM A APLICACAO. AQUI E APENAS UMA REFERENCIA
   if(NomeProg.isEmpty()) {
      QImage logo(":/docbase.png");
      int _x = (geometry().width() - logo.width() )/2;
      int _y = (geometry().height() - logo.height() )/2;
      painter.save();
      painter.translate(_x,_y);
      painter.drawImage(0,0,logo);
      painter.restore();
   }
   else {
      painter.setPen(QColor(230,230,230)); // Cinza 10%
      QFont arialLogo("Arial", 65, QFont::Bold);
      QFontMetricsF fmL(arialLogo);
      painter.setFont(arialLogo);
      int x = (geometry().width() - fmL.width(NomeProg) ) /2;
      int y = (geometry().height() - fmL.height() ) /2;
      painter.drawText(x,y,NomeProg);
   }

   QImage vcp(":/logo-vcp.jpg");
   int x0 = (geometry().width() - vcp.width() );
   painter.save();
   painter.translate(x0,10);
   painter.drawImage(0,0,vcp);
   painter.restore();

}
