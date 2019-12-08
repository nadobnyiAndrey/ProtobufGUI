#ifndef ENUMPROTOITEM_H
#define ENUMPROTOITEM_H

#include <QItemDelegate>

#include "prototreeitem.h"

class EnumProtoItem : public ProtoTreeItem
{
    class ItemDelegate : public QItemDelegate
    {
    public:
        ItemDelegate(const QStringList& items, QObject * parent = nullptr);

        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        void setEditorData(QWidget *editor, const QModelIndex &index) const override;
        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    private:
        QStringList mItems;
    };

public:
    EnumProtoItem( const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);
    ~EnumProtoItem() override;
    // ProtoTreeItem interface
public:
    QItemDelegate *getDelegate() const override;
    QBrush color() const override;
    void initFieldValue(const google::protobuf::Message *) override;

protected:
    void addFieldValue(google::protobuf::Message *message, const google::protobuf::FieldDescriptor *desc) override;
    void setFieldValue(google::protobuf::Message *message) override;

private:
    ItemDelegate *mDelegate;
};

#endif // ENUMPROTOITEM_H
