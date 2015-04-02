#include <QtNetwork>
#include <QDebug>

#include "server-browser.hpp"

ROK::ServerBrowser::ServerBrowser()
{
	this->url = QUrl("http://store.codehatch.com/rok/lobby.php");
	
	this->toolbar = new QToolBar();
	this->serverListView = new QTableView();
	this->mainLayout = new QVBoxLayout();
	this->mainLayout->addWidget(toolbar);
	this->mainLayout->addWidget(serverListView);
	this->setLayout(mainLayout);
	
	QAction *refreshAction = new QAction(QApplication::translate("reload", "Reload"), this);
	
	connect(refreshAction, SIGNAL(triggered()), this, SLOT(startRequest()));
	
	this->toolbar->addAction(refreshAction);

	/*
	this->model.setHorizontalHeaderLabels(
		QStringList()
			<< QApplication::translate("servertitle", "Server Title")
			<< QApplication::translate("playercount", "Player Count")
			<< QApplication::translate("maxplayers", "Max. Players")
	);
	*/

	this->setWindowTitle(QApplication::translate("rokserverbrowser", "Reign Of Kings Server Browser"));
}

void ROK::ServerBrowser::startRequest()
{
	QNetworkRequest request;
	
	request.setUrl(this->url);
	reply = this->qnam.get(request);
	QObject::connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
	QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
	QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateDataReadProgress(qint64, qint64)));
}

void ROK::ServerBrowser::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
	if (httpRequestAborted)
		return;
	
	qDebug() << bytesRead << "/" << totalBytes;
}

void ROK::ServerBrowser::httpReadyRead()
{
	// this slot gets called every time the QNetworkReply has new data.
	// We read all of its new data and write it into the file.
	// That way we use less RAM than when reading it at the finished()
	// signal of the QNetworkReply
	//qDebug() << "Retrieved" << this->reply->readAll().size();
	this->parseServerList(this->reply->readAll());
}

void ROK::ServerBrowser::cancelDownload()
{
	this->httpRequestAborted = true;
	this->reply->abort();
}

void ROK::ServerBrowser::httpFinished()
{
	if (this->httpRequestAborted)
	{
		this->reply->deleteLater();
		
		return;
	}
	
	if (this->reply->error() == QNetworkReply::NoError)
	{
		// Download succeeded
		qDebug() << "Success" << this->reply->readAll().count();
	}
	
	else
	{
		// Download failed
		// reply->errorString()
		qDebug() << "Error" << this->reply->errorString();
	}
	
	this->reply->deleteLater();
	this->reply = 0;
	
	qDebug() << this->listStatus;
	
	foreach (QStringList row, this->serverList)
	{
		QList<QStandardItem *> items;
		
		foreach (QString text, row)
			items.append(new QStandardItem(text));
		
		this->model.appendRow(items);
	}
	
	this->serverListView->setModel(&this->model);
	this->serverListView->verticalHeader()->hide();
	this->serverListView->resizeColumnsToContents();
	// Stretch first column horizontally to fill remaining space
	this->serverListView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void ROK::ServerBrowser::parseServerList(QByteArray serverListData)
{
	// Get status at the beginning up to delimiter
	if (this->serverList.length() == 0 && serverListData.startsWith(0x06))
	{
		qDebug() << "Parsing first data";
		// Search bytes need to be null (0x00) terminated to get a C string!
		const char endOfStatus[] = { 0x0A, 0x1C, 0x00 };
		int endIndex = serverListData.indexOf(endOfStatus);
		if (endIndex != -1)
		{
			this->listStatus = QString(serverListData.mid(1, endIndex - 1));
			
			serverListData.remove(0, endIndex + 2);
		}
		
		else
		{
			qDebug() << "Error finding the status of the list" << serverListData.toHex();
			
			return;
		}
	}
	
	const char endOfListItem[] = { 0x1D, 0x0A, 0x00 };
	QList<QByteArray> listItems;
	
	// Prepend the rest data from the previous chunk to complete it with data from this one
	serverListData.prepend(this->rest);
	
	int nextItemEnd = serverListData.indexOf(endOfListItem);
	int test = 0;
	
	while (nextItemEnd != -1)
	{
		//qDebug() << "List item:" << serverListData.left(nextItemEnd).toHex();
		this->parseServer(serverListData.mid(1, nextItemEnd));
		listItems.append(serverListData.mid(1, nextItemEnd));
		serverListData.remove(0, nextItemEnd + 2);
		nextItemEnd = serverListData.indexOf(endOfListItem);
		test++;
	}
	
	// Save the "rest" for which no delimiter could be found
	this->rest = serverListData;
	
	//qDebug() << "List items:" << listItems.length();
	//qDebug() << "Loop:" << test;
	//qDebug() << "First item:" << listItems.at(0).toHex();
	//qDebug() << "Last item:" << listItems.at(listItems.length() - 1).toHex();
	//qDebug() << "Rest:" << this->rest.toHex();
}

void ROK::ServerBrowser::parseServer(QByteArray listItem)
{
	const char fieldSep[] = { 0x1F, 0x00 };
	QStringList server;
	int nextFieldEnd = listItem.indexOf(fieldSep);
	
	//qDebug() << "First character:" << listItem.at(0);
	
	while (nextFieldEnd != -1)
	{
		server.append(listItem.left(nextFieldEnd));
		listItem.remove(0, nextFieldEnd + 1);
		nextFieldEnd = listItem.indexOf(fieldSep);
	}
	
	if (server.length() != 14)
		qDebug() << "Warning:" << "Omitting possibly wrong entry with" << server.length() << "columns.";
	else	
		this->serverList.append(server);
}