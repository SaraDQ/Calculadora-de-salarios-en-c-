#ifndef OBRERO_H
#define OBRERO_H

#include <QObject>

enum class TipoJornada
{
    Matutina,
    Vespertina,
    Nocturna
};

class Obrero : public QObject
{
    Q_OBJECT
public:
    explicit Obrero(QObject *parent = nullptr);
    Obrero(const QString &nombre, int horas, TipoJornada jornada);

    const QString &nombre() const;
    void setNombre(const QString &newNombre);

    int horas() const;
    void setHoras(int newHoras);

    TipoJornada jornada() const;
    void setJornada(TipoJornada newJornada);

    double salarioBruto() const;
    void setSalarioBruto(double newSalarioBruto);
    double salarioNeto() const;
    void setSalarioNeto(double newSalarioNeto);
    double descuento() const;
    void setDescuento(double newDescuento);

    void setSumaSB(double newSB);
    void setSumaSN(double newSN);
    void setSumaDes(double newDes);

    QString toString();

    QString toStringSumaSBTotal();
    QString toStringSumaSNTotal();
    QString toStringSumaDesTotal();

signals:

private:
    QString m_nombre;
    int m_horas;
    TipoJornada m_jornada;
    double m_salarioBruto;
    double m_salarioNeto;
    double m_descuento;

    double m_sumaTotalSB;
    double m_sumaTotalSN;
    double m_sumaTotalDes;

    QString jornada2String();

};

#endif // OBRERO_H
