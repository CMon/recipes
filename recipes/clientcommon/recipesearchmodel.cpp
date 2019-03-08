#include "recipesearchmodel.h"

#include <cereal/archives/json.hpp>
#include <rpclib/client/rpcclient.h>

#include <QJsonDocument>
#include <QJsonObject>

RecipeSearchModel::RecipeSearchModel(RPCClient * rpcClient)
    : rpcClient_(rpcClient)
{
	connect(rpcClient_, &RPCClient::newMessageArrived, this, &RecipeSearchModel::handleMessageFromServer);
}

void RecipeSearchModel::search(const QString & searchTerm)
{
	QJsonObject searchData;
	searchData.insert("searchTerm", searchTerm);

	rpcClient_->messageToServer("QList<Recipe> RecipeService::searchRecipes(QString)", searchData);
}

int RecipeSearchModel::rowCount(const QModelIndex &) const
{
	return recipes_.size();
}

QVariant RecipeSearchModel::data(const QModelIndex & index, int role) const
{
	if (index.row() < 0 || index.row() >= rowCount(index)) return QVariant();

	const int row = index.row();

	switch(role) {
		case ImageRole: return recipes_.at(row).getImagePath();
		case TitleRole: return recipes_.at(row).getTitle().value("de");
	}

	return QVariant();
}

QHash<int, QByteArray> RecipeSearchModel::roleNames() const
{
	QHash<int, QByteArray> retval;
	retval[ImageRole] = "image";
	retval[TitleRole] = "title";
	return retval;
}

void RecipeSearchModel::handleMessageFromServer(const QVariantHash & map)
{
	const QJsonObject message = QJsonObject::fromVariantHash(map);

	if (message.contains("signature")) {
		const std::string str = QJsonDocument(message.value("data").toObject()).toJson().toStdString();
		handleCallbackResponse(message.value("signature").toString(), str);
	}
}

void RecipeSearchModel::handleCallbackResponse(const QString & signature, const std::string & string)
{
	std::stringstream stream;
	stream.str(string);
	cereal::JSONInputArchive archive(stream);

	if (signature == "QList<Recipe> RecipeService::searchRecipes(QString)") {
		beginResetModel();
		recipes_.clear();
		archive(recipes_);
		qInfo() << "updated model to show" << recipes_.size() << "recipes";
		endResetModel();
	}
}
