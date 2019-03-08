#ifndef RECIPESEARCHMODEL_H
#define RECIPESEARCHMODEL_H

#include <QAbstractListModel>
#include <recipes/common/recipe.h>

class RPCClient;

class RecipeSearchModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum AbonementListRoles {
		ImageRole = Qt::UserRole + 1,
		TitleRole
	};

	RecipeSearchModel(RPCClient * rpcClient);

	Q_INVOKABLE void search(const QString & searchTerm);

public:
	int rowCount(const QModelIndex &) const override;
	QVariant data(const QModelIndex & index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;

private slots:
	void handleMessageFromServer(const QVariantHash & map);

private:
	void handleCallbackResponse(const QString & signature, const std::string & string);

private:
	RPCClient * rpcClient_;
	QList<Recipe> recipes_;
};

#endif // RECIPESEARCHMODEL_H
