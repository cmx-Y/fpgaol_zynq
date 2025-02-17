#include "wsserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QJsonDocument>
#include <exception>
#include <iostream>

QT_USE_NAMESPACE

wsServer::wsServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("FPAGOL WebSocket Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_debug(debug)
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            qDebug() << "FPAGOL WebSocket Server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &wsServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &wsServer::closed);
    } else {
		throw std::runtime_error("Ws server failed to start!");
	}
}

wsServer::~wsServer()
{
    m_pWebSocketServer->close();
    qDebug() << "wsServer DISCONNECTED!";
    // qDeleteAll(uart_clients.begin(), uart_clients.end());
    qDeleteAll(FPGA_clients.begin(), FPGA_clients.end());
}

void wsServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    QString req_url = pSocket->requestUrl().toString();

    /*if (req_url.endsWith("/uartws/")) {
        qDebug() << "uart ws connected";
        connect(pSocket, &QWebSocket::textMessageReceived, this, &wsServer::recvUartMessage);
        connect(pSocket, &QWebSocket::binaryMessageReceived, this, &wsServer::processBinaryMessage);
        connect(pSocket, &QWebSocket::disconnected, this, &wsServer::uartSocketDisconnected);
        uart_clients << pSocket;
    } else */if (req_url.endsWith("/ws/")) {
        qDebug() << "FPGA ws connected";
        connect(pSocket, &QWebSocket::textMessageReceived, this, &wsServer::recvFGPAMessage);
        connect(pSocket, &QWebSocket::binaryMessageReceived, this, &wsServer::processBinaryMessage);
        connect(pSocket, &QWebSocket::disconnected, this, &wsServer::FPGASocketDisconnected);
        FPGA_clients << pSocket;
    }
}

void wsServer::sendFPGAMessage(QString message) {
    for (QWebSocket *pClinet : FPGA_clients) {
        pClinet->sendTextMessage(message);
    }
}

// void wsServer::sendUartMessage(QString message) {
//     for (QWebSocket *pClinet : uart_clients) {
//         pClinet->sendTextMessage(message);
//     }
// }

// void wsServer::recvUartMessage(QString message)
// {
//     QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
//     if (m_debug)
//         qDebug() << "uart Message received:" << message;
//     if (pClient) {
// 		auto json = QJsonDocument::fromJson(message.toUtf8());
// 		QString msg = json["msg"].toString();
// 		emit uart_write((msg + "\n").toUtf8());
//     }
// }

void wsServer::recvFGPAMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
		auto json = QJsonDocument::fromJson(message.toUtf8());

        //qDebug() << "json is object: " << json.object()["gpio"];
        //qDebug() << "json to int: " << json.object()["gpio"].toInt();
		if (m_debug)
			qDebug() << "FPGA Message received: GPIO: " << json.object()["gpio"] << " level: " << json.object()["level"];

        

		int gpio = json.object()["gpio"].toInt();
        int level;
        QString msg;

		int ret;

        if(gpio == -2){
            ret = emit notify_start();

            if (m_debug)
                qDebug() << "Start Notify returned: " << ret;
        }else if(gpio >= 0){
            level = (int)json.object()["level"].toBool();
            emit gpio_write(gpio, level);
        }

        /* if(gpio == -1){
            qDebug() << "END notify";
            if (emit notify_end() == 0) {
                QJsonObject reply;
                QJsonArray _v;
                level = (int)json.toJson()["level"].toBool();
                if (level == 0){
                    qDebug() << "Level: " << json.toJson()["level"].toBool();
                    reply["type"] = "WF";
                    reply["values"] = _v;
                    auto msg = QJsonDocument(reply).toJson(QJsonDocument::Compact);

                    qDebug() << "Send: " << msg;

                    pClient->sendTextMessage(msg);
                }
            }
        }
        else if(gpio == -2){
            ret = emit notify_start();

            if (m_debug)
                qDebug() << "Start Notify returned: " << ret;
        }
        else if(gpio == -3){
            msg = (QString)json.toJson()["level"].toString();
            printf("Input : %s\n",msg.toStdString().data());
            ret = emit uart_write((msg).toUtf8());
        }
        else if(gpio >= 0){
            level = (int)json.toJson()["level"].toBool();
            emit gpio_write(gpio, level);
        }
        else{
            qDebug() << "WARNING: reach undefined place!!!!!!!!!!!!!!!";
        } */
        // pClient->sendTextMessage(message);
    }
}

void wsServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

// void wsServer::uartSocketDisconnected()
// {
//     QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
//     if (m_debug)
//         qDebug() << "uart socketDisconnected:" << pClient;
//     if (pClient) {
//         uart_clients.removeAll(pClient);
//         pClient->deleteLater();
//     }
// }

void wsServer::FPGASocketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "FPGA socketDisconnected:" << pClient;
    if (pClient) {
		emit notify_end();

        FPGA_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
