#include <QtGui>
#include <QPushButton>
#include <QCheckBox>
#include <QIntValidator>

#include "gogui.h"
#include "GameScene.h"
#include "ResizableView.h"
#include "SquareGrid.h"

GoGui::GoGui(Engine& engine, QWidget *parent ) :
    QDialog(parent)
{
    initView (engine);
}

void GoGui::initView(Engine& engine)
{
    GameScene *gameScene = GameScene::createGoScene(9);

    m = new Manager(engine, gameScene, this);
    m->newGame();

    ResizableView *gameView = new ResizableView(gameScene, this);

    QPushButton *new_game = new QPushButton("New game");
    QPushButton *gen_move = new QPushButton("Gen move");

    QPushButton *play_move = new QPushButton("Play move");
    QPushButton *undo_move = new QPushButton("Undo move");

    QCheckBox *show_gammas = new QCheckBox("Show gammas");

    QPushButton *set_komi = new QPushButton("Set komi");

    QPushButton *quit = new QPushButton("Quit");

    QVBoxLayout *controls = new QVBoxLayout;
    controls->addWidget(new QLabel("<b>GoGui</b>"), 0, Qt::AlignHCenter);

    controls->addWidget(new_game);
    controls->addWidget(gen_move);
    controls->addWidget(play_move);
    controls->addWidget(undo_move);
    controls->addWidget(show_gammas);
    controls->addWidget(set_komi);
    controls->addWidget(quit);
    controls->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(gameView);
    mainLayout->addLayout(controls);

    connect(new_game, SIGNAL(clicked()), m, SLOT(newGame()));
    connect(gen_move, SIGNAL(clicked()), m, SLOT(genMove()));
    connect(play_move, SIGNAL(clicked()), m, SLOT(playMove()));
    connect(undo_move, SIGNAL(clicked()), m, SLOT(undoMove()));
    connect(show_gammas, SIGNAL(stateChanged(int)), m, SLOT(showGammas(int)));
    connect(set_komi, SIGNAL(clicked()), this, SLOT(setKomi()));
    connect(quit, SIGNAL(clicked()), this, SLOT(close()));

    connect(m, SIGNAL(stateChanged(const Player&)), this, SLOT(setWinner(const Player&)));
    connect(m, SIGNAL(statusChanged(QString)), this, SLOT(setStatus(QString)));

    statebar = new QLabel(" ");
    QVBoxLayout *all_ = new QVBoxLayout;
    all_->addLayout(mainLayout);
    all_->addWidget(statebar);

    setLayout(all_);
}

void GoGui::setKomi() {
    bool ok;
    double komi = QInputDialog::getDouble(
            this, "Set komi", "Enter double:", 
            10, 0, 100, 2, &ok);
    if (ok)
        m->setKomi(komi);
}

void GoGui::setWinner(const Player&) {
}

void GoGui::setStatus(QString s) {
    statebar->setText(s);
}

