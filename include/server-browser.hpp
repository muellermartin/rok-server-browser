#ifndef SERVER_BROWSER_HPP
#define sSERVER_BROWSER_HPP

#include <QWidget>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QtWidgets>

namespace ROK
{
	class ServerBrowser : public QWidget
	{
		Q_OBJECT
		
		private:
			QNetworkAccessManager qnam;
			QUrl url;
			QNetworkReply *reply;
			QToolBar *toolbar;
			QTableView *serverListView;
			QVBoxLayout *mainLayout;
			int httpGetId;
			bool httpRequestAborted;
			QList<QStringList> serverList;
			QStandardItemModel model;
			QString listStatus;
			QByteArray rest;
		
		private slots:
			void startRequest();
			void cancelDownload();
			void httpFinished();
			void httpReadyRead();
			void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
		
		public:
			ServerBrowser();
			void parseServerList(QByteArray serverListData);
			void parseServer(QByteArray listItem);
	};
}

#endif // SERVER_BROWSER_HPP