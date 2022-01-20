#include "salarios.h"
#include "ui_salarios.h"

Salarios::Salarios(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Salarios)
{
    ui->setupUi(this);
    m_controlador = new Controlador();
}

Salarios::~Salarios()
{
    delete ui;
}


void Salarios::on_cmdCalcular_clicked()
{
    calcular();
}

void Salarios::limpiar()
{
    ui->inNombre->setText("");
    ui->inHoras->setValue(0);
    ui->inMatutino->setChecked(true);
    ui->inNombre->setFocus();
}

void Salarios::guardar()
{
    // Abrir cuadro de diálogo para seleccionar ubicación y nombre del archivo.
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar datos",
                                                         QDir::home().absolutePath(),
                                                         "Archivos de texto (*.txt)");
    //qDebug() << nombreArchivo;

    // Crear un objeto QFile
    QFile archivo(nombreArchivo);
    // Abrirlo para escritura
    if(archivo.open(QFile::WriteOnly | QIODevice::Text)){
        // Crear un 'stream' de texto
        QTextStream salida(&archivo);
        // Enviar los datos del resultado a la salida
        salida << ui->outResultado->toPlainText()<<"\n";
        salida << ui->outSalarioBruto->text()<<"\n";
        salida << ui->outIESS->text()<<"\n";
        salida << ui->outSalarioNeto->text()<<"\n";
        // Mostrar 5 segundo que todo fue bien
        ui->statusbar->showMessage("Datos almacenados en " + nombreArchivo, 5000);
    }else {
        // Mensaje de error si no se puede abrir el archivo
        QMessageBox::warning(this,
                             "Guardar datos",
                             "No se pudo guardar los datos");
    }
    // Cerrar el archivo
    archivo.close();

}

void Salarios::abrir()
{
    // Abrir cuadro de diálogo para seleccionar ubicación y nombre del archivo.
    QString nombreArchivo = QFileDialog::getOpenFileName(this,
                                                         "Abrir archivo",
                                                         QDir::home().absolutePath(),
                                                         "Archivos de texto (*.txt)");
    QFile archivo(nombreArchivo);
    // Abrirlo para escritura
    if(archivo.open(QFile::ReadOnly)){
        // Crear un 'stream' de texto
        QTextStream entry(&archivo);
        //leer todo el contenido del archivo
        QString data="",name;

        while (name!="_"&& entry.atEnd()==false) {
            name=entry.readAll();
            if(name=="_"){
            }else{
                data=data+name+"\n";
            }
        }
        //cargar el contenido
        ui->outSalarioBruto->clear();
        ui->outIESS->clear();
        ui->outSalarioNeto->clear();
        ui->outResultado->clear();
        //Aqui se van a mostrar los resultados obtenidos al calcular los totales
        ui->outResultado->setPlainText(data);
        //Mostrar el valor que se le asigno al total de salario bruto
        entry.readLine();
        QString totalSB=entry.readLine();
        m_controlador->setSBT(totalSB.toDouble());
        ui->outSalarioBruto->setText(totalSB);

        //Mostrando el valor que se le asigno a descuento
        QString totalDes=entry.readLine();
        m_controlador->setDesT(totalDes.toDouble());
        ui->outIESS->setText(totalDes);

        //Mostrando el valor que se le asigno a salario neto
        QString totalSN=entry.readLine();
        m_controlador->setSNT(totalSN.toDouble());
        ui->outSalarioNeto->setText(totalSN);

        // Mostrar 5 segundos que todo fue bien
        ui->statusbar->showMessage("Datos leidos desde " + nombreArchivo, 5000);
    }else {
        // Mensaje de error si no se puede abrir el archivo
        QMessageBox::warning(this,
                             "Abrir datos",
                             "No se pudo abrir los datos");
    }
    // Cerrar el archivo
    archivo.close();
}


void Salarios::on_actionCalcular_triggered()
{
    calcular();
}

void Salarios::calcular()
{

    // Obteber datos de la GUI
    QString nombre = ui->inNombre->text();
    int horas = ui->inHoras->value();
    TipoJornada jornada;
    if (ui->inMatutino->isChecked()){
        jornada = TipoJornada::Matutina;
    } else if (ui->inVespertina->isChecked()){
        jornada = TipoJornada::Vespertina;
    } else {
        jornada = TipoJornada::Nocturna;
    }

    // Validar datos correctos
    if (nombre == "" || horas == 0){
        /*
        QMessageBox msgBox;
        msgBox.setText("El nombre o el número de horas está vacío");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        */
        QMessageBox::warning(this,"Advertencia","El nombre o el número de horas está vacío");
        return;
    }

    // Agregar obrero al controlador
    m_controlador->agregarObrero(nombre, horas, jornada);
    // Calcular
    if (m_controlador->calcularSalario()){
        // muestra los resultados de los calculos del obrero
        ui->outResultado->appendPlainText(m_controlador->obrero()->toString());
        ui->outSalarioBruto->setText(m_controlador->obrero()->toStringSumaSBTotal());
        ui->outIESS->setText(m_controlador->obrero()->toStringSumaDesTotal());
        ui->outSalarioNeto->setText(m_controlador->obrero()->toStringSumaSNTotal());
        // limpiar la interfaz
        limpiar();
        // Mostrar mensaje por 5 segundos en la barra de estado
        ui->statusbar->showMessage("calculos procesados para " + nombre, 5000);
    }else {
        QMessageBox::critical(
                    this,
                    "Error",
                    "Error al calcular el salario.");
    }

}


void Salarios::on_actionGuardar_triggered()
{
    guardar();
}


void Salarios::on_actionNuevo_triggered()
{
    limpiar();
    ui->outResultado->clear();
    ui->outIESS->clear();
    ui->outSalarioBruto->clear();
    ui->outSalarioNeto->clear();
}


void Salarios::on_actionAbrir_triggered()
{
    abrir();
}


void Salarios::on_actionAcerca_de_Salarios_triggered()
{
    //crear un objeto de la ventana qe queremos invocar
    Acerca *dialogo = new Acerca(this);
    //Enviar parametro a la ventana
    dialogo->setVersion(VERSION);
    //mostrar el cuadro de dialogo
    dialogo->exec();
    //obtener datos de la ventana
    dialogo->valor();
}


void Salarios::on_actionSalir_triggered()
{
    close();
}


