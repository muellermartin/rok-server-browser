#include <QtWidgets>
#include <QApplication>

#include "server-browser.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
/*
	QWidget window;
	QTableView *serverList = new QTableView();

	window.setWindowTitle(QApplication::translate("rokserverbrowser", "Reign Of Kings Server Browser"));
	window.show();
	
	QToolBar *toolbar = new QToolBar();
	
	toolbar->addAction(QApplication::translate("reload", "Reload"));
	toolbar->show();
	
	QVBoxLayout *mainLayout = new QVBoxLayout();
	
	mainLayout->addWidget(toolbar);
	mainLayout->addWidget(serverList);
	window.setLayout(mainLayout);
	
	QStandardItemModel model;
	
	model.setHorizontalHeaderLabels(
		QStringList()
			<< QApplication::translate("servertitle", "Server Title")
			<< QApplication::translate("playercount", "Player Count")
			<< QApplication::translate("maxplayers", "Max. Players")
			<< QApplication::translate("ping", "Ping")
	);
	
	QList<QStringList> rows = QList<QStringList>()
		<< (QStringList() << "Foo" << "1" << "32" << "123")
		<< (QStringList() << "Bar" << "0" << "16" << "321");
	
	foreach (QStringList row, rows)
	{
		QList<QStandardItem *> items;
		
		foreach (QString text, row)
			items.append(new QStandardItem(text));
		
		model.appendRow(items);
	}
	
	serverList->setModel(&model);
	serverList->verticalHeader()->hide();
	serverList->resizeColumnsToContents();
	// Stretch first column horizontally to fill remaining space
	serverList->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
*/
	ROK::ServerBrowser serverBrowser;
	
	serverBrowser.show();
	
	return app.exec();
} 
