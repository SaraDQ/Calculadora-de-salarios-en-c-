#include "controlador.h"

Controlador::Controlador(QObject *parent) : QObject(parent)
{

    this->m_obrero = nullptr;
}

bool Controlador::agregarObrero(QString nombre, int horas, TipoJornada jornada)
{
    this->m_obrero = new Obrero(nombre,horas,jornada);
    return true;
}


bool Controlador::calcularSalario()
{
    // Validar que el obrero no sea nulo
    if (m_obrero == nullptr)
        return false;

    // Deternimar el valor hora
    double valorHora = 0;
    switch(m_obrero->jornada()){
    case TipoJornada::Vespertina:
        valorHora = VESPERTINO;
        break;
    case TipoJornada::Matutina:
        valorHora = MATITUNO;
        break;
    case TipoJornada::Nocturna:
        valorHora = NOCTURNO;
        break;
    default:
        return false;
    }
    // Calculo de la horas extra
    int horas = m_obrero->horas();
    int horasExtra = 0;
    if (m_obrero->horas() > 40){
        horasExtra = m_obrero->horas() - 40;
        horas = 40;
    }
    // Calculo del salario
    double salarioBruto = horas * valorHora + horasExtra * (HORA_EXTRA * valorHora / 100);
    // Calculo del descuento
    double descuento = salarioBruto * IESS / 100;
    // Calculo del salario neto (a recibir)
    double salarioNeto = salarioBruto - descuento;
    // Establecer valores al objeto
    m_obrero->setSalarioBruto(salarioBruto);
    m_obrero->setDescuento(descuento);
    m_obrero->setSalarioNeto(salarioNeto);

    m_sumTotalBruto+=salarioBruto;
    m_sumTotalIESS+=descuento;
    m_sumTotalNeto+=salarioNeto;
    //Establecer valores al objeto
    m_obrero->setSumaSB(m_sumTotalBruto);
    m_obrero->setSumaSN(m_sumTotalNeto);
    m_obrero->setSumaDes(m_sumTotalIESS);
    // Retornar true (todo se calculo correctamente)
    return true;
}

Obrero *Controlador::obrero() const
{
    return m_obrero;
}

void Controlador::setObreroCal(Obrero *newObrero)
{
    m_obrero = newObrero;
}

void Controlador::setSBT(double newGTotal)
{
    m_sumTotalBruto=newGTotal;
}

void Controlador::setSNT(double newGTotal)
{
    m_sumTotalNeto=newGTotal;
}

void Controlador::setDesT(double newGTotal)
{
   m_sumTotalIESS=newGTotal;
}
