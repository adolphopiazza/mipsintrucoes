#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <iostream>
#include <string.h>
#include <bitset>
#include <sstream>

using namespace std;

int identificaInstrucao (string binario);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Desativando o resize
        setMinimumSize(QSize(400,492));
        setMaximumSize(QSize(400,492));

    ui->totalCiclosLbl->setText("");
    ui->totalInstrucoesLbl->setText("");
    ui->totalILbl->setText("");
    ui->totalJLbl->setText("");
    ui->totalRLbl->setText("");
    ui->totalLWLbl->setText("");
    ui->totalCPILbl->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectButton_clicked() {
    //Abre o dialog para selecionar o arquivo
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/path/to/file/");
    ui->pathEdit->setText(fileName);
}

void MainWindow::on_analyseButton_clicked() {

    //Verifica se tem algo no lineEdit
    if (ui->pathEdit->text() == "")
        ui->pathEdit->setText("Insira o PATH");

    //Logica principal
    ifstream myFile;
    string line;
    stringstream ss;
    unsigned n;
    int totalCiclos = 0;
    int totalInstrucoes = 0;
    int formatoJ = 0;
    int formatoI = 0;
    int formatoR = 0;
    int LW = 0;
    float cpi;

    myFile.open(QString(ui->pathEdit->text()).toUtf8(), ios::in | ios::binary);

    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            ss << hex << line;
            ss >> n;
            bitset<32> b(n);
            cout << "Binario: " << b.to_string() << endl;
            totalCiclos += identificaInstrucao(b.to_string()) % 10;
            formatoJ += (identificaInstrucao(b.to_string())/10 == 3) ? 1:0;
            formatoI += (identificaInstrucao(b.to_string())/10 == 2) ? 1:0;
            formatoR += (identificaInstrucao(b.to_string())/10 == 1) ? 1:0;
            LW       += (identificaInstrucao(b.to_string())/10 == 4) ? 1:0;
            totalInstrucoes++;
        }
    }

    //Calcular o CPI
    cpi = (1.0f) * totalCiclos/totalInstrucoes;

    myFile.close();

    ui->totalCiclosLbl->setText("Total de ciclos: " + QString::number(totalCiclos));
    ui->totalInstrucoesLbl->setText("Total de instruções: " + QString::number(totalInstrucoes));
    ui->totalILbl->setText("Total formato I: " + QString::number(formatoI));
    ui->totalJLbl->setText("Total formato J: " + QString::number(formatoJ));
    ui->totalRLbl->setText("Total formato R: " + QString::number(formatoR));
    ui->totalLWLbl->setText("Total de LW: " + QString::number(LW));
    ui->totalCPILbl->setText("CPI: " + QString::number(cpi));


}

int identificaInstrucao (string binario) {

    string opcode = binario.substr(0, 6);

    //1 na frente = formato R
    //2 na frente = formato I
    //3 na frente = formato J
    //4 na frente = LW

    if (opcode == "000000") {return 14;}
    else if (opcode == "001000") {return 24;}
    else if (opcode == "001001") {return 24;}
    else if (opcode == "000100") {return 23;}
    else if (opcode == "000101") {return 23;}
    else if (opcode == "000010") {return 33;}
    else if (opcode == "000011") {return 33;}
    else if (opcode == "001111") {return 24;}
    else if (opcode == "100011") {return 45;}
    else if (opcode == "001101") {return 24;}
    else if (opcode == "001010") {return 24;}
    else if (opcode == "101011") {return 24;}

    return 0;

}
