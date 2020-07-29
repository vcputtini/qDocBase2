#include "cnpj.h"

CNPJ::CNPJ(QString cnpj) :
    _cnpj(cnpj.simplified()),
    _dv(0),
    _valid(false)
{
    normalize_P();
}

// operator overload
/**
 * @brief CNPJ::operator =
 *        Atribuicao de valores entre tipos CNPJ
 * @param cnpj2
 * @return
 */
CNPJ CNPJ::operator=(CNPJ cnpj2)
{
    if(this == &cnpj2)
        return *this;

    if(_valid)
        _cnpj = cnpj2._cnpj;

    return *this;
}

/**
 * @brief CNPJ::operator ==
 *        Esta sobrecarga compara byte a byte entre
 *        dois CNPJ.
 *        Se qualquer um dos bytes forem diferentes
 *        interrompe o processamento e retorna false
 * @param cnpj2
 * @return true | false
 */
bool CNPJ::operator==(const CNPJ &cnpj2) const
{
    if(_valid) {
        bool rc=true;
        QString temp = _cnpj;
        for(int i=0; i<14; i++) {
            if(temp[i] != cnpj2._cnpj[i]) {
                rc = false;
                break;
            }
        }
        return rc;
    }
    return false;
}

/**
 * @brief CNPJ::operator !=
 *        Oferece um modo de comparacao adicional, alem
 *        do igual (==)
 * @param cnpj2
 * @return true | false
 */
bool CNPJ::operator!=(const CNPJ &cnpj2) const {
   return !(*this == cnpj2);
}
// end - operator overload

/**
 * @brief CNPJ::data
 *        Retorna o valor do CNPJ passado via o construtor do tipo.
 *        Permite que o usuario escolha entre dois formatos
 *        de apresentacao
 * @param format
 * @return QString(CNPJ)
 */
QString CNPJ::data(CNPJFormats format)
{
    if(_valid) {
        stDelimiter stDel[][2] {
            { 2, "." },
            { 6, "." },
            { 10, "/" },
            { 15, "-" }
        };

        switch(format) {
        case NumericOnly:
            if(_cnpj.count(".")==2 && _cnpj.count("/")==1 && _cnpj.count("-")==1) {
                _cnpj.replace(".","").replace("/","").replace("-","");
            }
            break;
        case Delimited:
            if(_cnpj.length() == 14) {
                for(int i=0; i<4;i++) {
                    _cnpj.insert(stDel[i][0].pos,stDel[i][0].delemiter);
                }
            }
            break;
        }
    }
    return _cnpj;
}

/**
 * @brief CNPJ::DV
 * @return inteiro representando DV
 */
int CNPJ::DV()
{
    return calcDV_P();
}

/**
 * @brief CNPJ::isValid
 *        Compara o DV informado pelo usuario com o
 *        calculado se forem diferentes retorna false
 *        representando que o CNPJ provavelmente e
 *        falso.
 * @return true | false
 */
bool CNPJ::isValid()
{
    if(_valid) {
        int _dv = _cnpj.right(2).toInt();
        if(_dv != calcDV_P()) {
            return false;
        }
        return true;
    }

    return _valid;
}

/**
 * @brief CNPJ::section
 *        Retorna as partes significativas do CNPJ
 *        via uma estrutura.
 *
 *        Membros:
 *        first = Numero do Registro
 *        secont = Numero da matriz ou filial
 *        dv   = Valor do digito verificador
 *
 * @return struct Section
 */
CNPJ::Section CNPJ::section()
{
    Section sec;
    if(_valid) {
        sec.first = _cnpj.left(8);
        sec.second = _cnpj.mid(8,4);
        sec.dv    = _cnpj.right(2);
    } else {
        sec.first = "";
        sec.second = "";
        sec.dv = "";
    }
    return sec;
}

/**
 * @brief CNPJ::normalize_P
 *        Efetua diversas checagens com o valor inforamado
 *        a fim se assegurar que o valor e valido para
 *        ser calculado. De maneira geral tenta se certificar
 *        o valor, e esta estando correto, o converte para o
 *        formato interno para ser calculado.
 */
void CNPJ::normalize_P()
{

    if(_cnpj.isEmpty()) {
        _valid = false;
        return;
    }

    /* testa se o cnpj passado e composto por
     * numeros na faixa de 00000000000000 ate 99999999999999
     */
    for(int i=0; i<10; i++) {
        if(_cnpj.count(QString::number(i))==_cnpj.length() ) {
            _valid = false;
            _cnpj = "CNPJ::invalid";
            return;
        }
    }

    if(_cnpj.length() < 14 || _cnpj.length() > 18) {
        _valid = false;
        _cnpj = "CNPJ::invalid";
        return;
    }

    if(_cnpj.contains(".") && _cnpj.contains("/") && _cnpj.contains("-")) {  // perhaps formated
        if(_cnpj.count(".")==2 && _cnpj.count("/")==1 && _cnpj.count("-")==1) { // right
            _valid = true;
            _cnpj.replace(".","").replace("/","").replace("-","");
        } else { // wrong
            _valid = false;
            _cnpj = "CNPJ::invalid";
        }
    }

    _valid = true;
}

/**
 * @brief CNPJ::calcDV_P
 *        Calcula o valor do digito verificador
 * @return inteiro com o valor do DV
 */
int CNPJ::calcDV_P()
{
    if(_cnpj.isEmpty())
        return false;

    QString aux = _cnpj.left(12);
    QString dv = _cnpj.right(2);

    int weight1[12] = {5,4,3,2,9,8,7,6,5,4,3,2};
    int weight2[13] = {6,5,4,3,2,9,8,7,6,5,4,3,2};

    /* DV 1 */
    int p[12];
    int sum=0;
    for(int i=0; i<12; i++) {
        p[i] = aux.mid(i,1).toInt() * weight1[i];
        sum += p[i];
    }
    int mod1 = sum%11;
    int dig1=0;
    if(mod1 < 2)
        dig1 = 0;
    else
        dig1 = 11-mod1;

    /* DV 2 */
    aux+=QString::number(dig1); // add DV 1
    sum = 0;
    int p1[13]; // 05 jan 2016: resolve o bug de loop infinito
    for(int i=0; i<13; i++) {
        p1[i] = aux.mid(i,1).toInt() * weight2[i];
        sum += p1[i];
    }
    int mod2 = sum%11;
    int dig2=0;
    if(mod2 < 2)
        dig2 = 0;
    else
        dig2 = 11-mod2;

    return QString("%0%1").arg(dig1).arg(dig2).toInt();
}
