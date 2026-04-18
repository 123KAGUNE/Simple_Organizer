#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QDialog>
#include <QListWidget>
#include <QComboBox>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    organizer = new Organizer();
    setupUI();
    refreshTable();
}

MainWindow::~MainWindow() {
    delete organizer;
}

void MainWindow::setupUI() {
    setWindowTitle("File Organizer");
    resize(950, 650);

    auto* central = new QWidget(this);
    setCentralWidget(central);
    auto* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Título
    auto* title = new QLabel("FILE ORGANIZER", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 32px; font-weight: bold; color: #00ddff; margin-bottom: 10px;");
    mainLayout->addWidget(title);

    // Barra de busca
    auto* searchLayout = new QHBoxLayout();
    auto* searchLabel = new QLabel("🔎 Buscar extensão:", this);
    searchLine = new QLineEdit(this);
    searchLine->setPlaceholderText("Digite a extensão para filtrar...");
    connect(searchLine, &QLineEdit::textChanged, this, &MainWindow::refreshTable);

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchLine);
    mainLayout->addLayout(searchLayout);

    // Tabela de regras
    ruleTable = new QTableWidget(this);
    ruleTable->setColumnCount(2);
    ruleTable->setHorizontalHeaderLabels({"Extensão", "Caminho de Destino"});
    ruleTable->horizontalHeader()->setStretchLastSection(true);
    ruleTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ruleTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ruleTable->setAlternatingRowColors(true);
    mainLayout->addWidget(ruleTable);

    // Botões
    auto* btnLayout = new QHBoxLayout();
    auto* btnAdd = new QPushButton("Adicionar Regras", this);
    auto* btnRemove = new QPushButton("Remover Selecionada", this);
    auto* btnOrganize = new QPushButton("Organizar Pasta Agora", this);
    auto* btnQuit = new QPushButton("Sair", this);

    btnAdd->setMinimumHeight(40);
    btnRemove->setMinimumHeight(40);
    btnOrganize->setMinimumHeight(40);
    btnQuit->setMinimumHeight(40);

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addRule);
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::removeRule);
    connect(btnOrganize, &QPushButton::clicked, this, &MainWindow::organizeFolder);
    connect(btnQuit, &QPushButton::clicked, this, &QWidget::close);

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnRemove);
    btnLayout->addWidget(btnOrganize);
    btnLayout->addWidget(btnQuit);
    mainLayout->addLayout(btnLayout);
}

void MainWindow::refreshTable(const QString &filter) {
    ruleTable->setRowCount(0);
    const auto& rules = organizer->getRules();

    for (const auto& rule : rules) {
        QString extQ = QString::fromStdString(rule.ext);
        QString pathQ = QString::fromStdString(rule.path.string());

        if (!filter.isEmpty() && !extQ.contains(filter, Qt::CaseInsensitive))
            continue;

        int row = ruleTable->rowCount();
        ruleTable->insertRow(row);
        ruleTable->setItem(row, 0, new QTableWidgetItem(extQ));
        ruleTable->setItem(row, 1, new QTableWidgetItem(pathQ));
    }

    if (ruleTable->rowCount() == 0) {
        ruleTable->setRowCount(1);
        ruleTable->setItem(0, 0, new QTableWidgetItem("Nenhuma regra encontrada"));
        ruleTable->setItem(0, 1, new QTableWidgetItem(""));
    }
}

void MainWindow::addRule() {
    QDialog dialog(this);
    dialog.setWindowTitle("Adicionar Várias Extensões");
    dialog.resize(520, 580);

    auto* layout = new QVBoxLayout(&dialog);
    layout->setSpacing(12);

    // Categoria
    auto* catLabel = new QLabel("Categoria:", &dialog);
    auto* categoryCombo = new QComboBox(&dialog);
    categoryCombo->addItems({"Imagens", "Vídeos", "Documentos", "Áudio",
                             "Compactados", "Código", "Outros"});

    layout->addWidget(catLabel);
    layout->addWidget(categoryCombo);

    // Lista de extensões
    auto* listLabel = new QLabel("Selecione as extensões (Ctrl + Clique para várias):", &dialog);
    auto* extList = new QListWidget(&dialog);
    extList->setSelectionMode(QAbstractItemView::MultiSelection);

    layout->addWidget(listLabel);
    layout->addWidget(extList);

    // Lambda para atualizar lista
    auto updateList = [&](int index) {
        extList->clear();
        QStringList items;
        if (index == 0) { // Imagens
            items << ".png" << ".jpg" << ".jpeg" << ".webp" << ".gif" << ".bmp" << ".tiff" << ".svg" << ".heic";
        } else if (index == 1) { // Vídeos
            items << ".mp4" << ".mkv" << ".avi" << ".mov" << ".webm" << ".flv" << ".m4v" << ".wmv";
        } else if (index == 2) { // Documentos
            items << ".pdf" << ".docx" << ".doc" << ".xlsx" << ".xls" << ".pptx" << ".txt" << ".odt" << ".md";
        } else if (index == 3) { // Áudio
            items << ".mp3" << ".wav" << ".flac" << ".ogg" << ".m4a" << ".aac" << ".opus";
        } else if (index == 4) { // Compactados
            items << ".zip" << ".rar" << ".7z" << ".tar.gz" << ".tar.xz" << ".bz2" << ".gz";
        } else if (index == 5) { // Código
            items << ".cpp" << ".h" << ".hpp" << ".py" << ".js" << ".ts" << ".java" << ".cs" << ".rs" << ".go" << ".lua";
        } else { // Outros
            items << ".torrent" << ".iso" << ".exe" << ".deb" << ".appimage" << ".blend" << ".psd" << ".xcf";
        }
        extList->addItems(items);
    };

    connect(categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), updateList);
    updateList(0);

    // Extensões extras
    auto* extraLabel = new QLabel("Extensões extras (separadas por espaço ou vírgula):", &dialog);
    auto* extraEdit = new QLineEdit(&dialog);
    extraEdit->setPlaceholderText("Ex: .blend .psd .xcf .custom");
    layout->addWidget(extraLabel);
    layout->addWidget(extraEdit);

    // Botões
    auto* btnBox = new QHBoxLayout();
    auto* btnOk = new QPushButton("✅ Adicionar Selecionadas", &dialog);
    auto* btnCancel = new QPushButton("Cancelar", &dialog);

    btnOk->setMinimumHeight(38);
    btnCancel->setMinimumHeight(38);

    btnBox->addWidget(btnOk);
    btnBox->addWidget(btnCancel);
    layout->addLayout(btnBox);

    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() != QDialog::Accepted) return;

    // Processar seleções
    QStringList selected;
    for (auto* item : extList->selectedItems()) {
        selected << item->text();
    }

    QString extraText = extraEdit->text().trimmed();
    if (!extraText.isEmpty()) {
        QStringList extras = extraText.split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
        for (QString& e : extras) {
            if (!e.startsWith('.')) e.prepend('.');
            selected << e;
        }
    }

    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Você não selecionou nenhuma extensão!");
        return;
    }

    QString dest = QFileDialog::getExistingDirectory(this,
        "Escolha o destino para estas " + QString::number(selected.size()) + " extensões");

    if (dest.isEmpty()) return;

    int added = 0;
    for (const QString& ext : selected) {
        if (organizer->addRule(dest.toStdString(), ext.toStdString())) {
            added++;
        }
    }

    QMessageBox::information(this, "Sucesso!",
                             QString("%1 extensões foram adicionadas com sucesso!").arg(added));

    refreshTable(searchLine->text());
}

void MainWindow::removeRule() {
    int row = ruleTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aviso", "Selecione uma regra para remover.");
        return;
    }

    QString ext = ruleTable->item(row, 0)->text();
    if (organizer->removeRule(ext.toStdString())) {
        QMessageBox::information(this, "Removido", "Regra removida com sucesso.");
        refreshTable(searchLine->text());
    } else {
        QMessageBox::warning(this, "Erro", "Não foi possível remover a regra.");
    }
}

void MainWindow::organizeFolder() {
    QString src = QFileDialog::getExistingDirectory(this, "Selecione a pasta para organizar");
    if (src.isEmpty()) return;

    int count = organizer->organize(src.toStdString());
    if (count >= 0) {
        QMessageBox::information(this, "Concluído!",
                                 QString("Organização finalizada!\n%1 arquivos foram movidos.").arg(count));
    } else {
        QMessageBox::warning(this, "❌ Erro", "Caminho inválido ou pasta não existe.");
    }
}