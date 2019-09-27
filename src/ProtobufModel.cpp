#include "ProtobufModel.h"
#include "prototreeitem.h"
#include "repeatedprotoitem.h"

void ProtobufModel::setProtoClass(const google::protobuf::Descriptor *protoclass)
{
    if(protoclass != nullptr)
    {
        emit beginResetModel();
        mRootItem = std::make_unique<ProtoTreeItem>(protoclass);
        mRootItem->expand();
        emit endResetModel();
    }
}

void ProtobufModel::onExpand(const QModelIndex &index)
{
    if (!mRootItem || !index.isValid())
        return;

    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());

    item->expand();
    beginInsertRows(index, 0, item->rowCount());
    endInsertRows();
}

void ProtobufModel::onAddItem(const QModelIndex &index)
{
    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());
    RepeatedProtoItem *rItem = dynamic_cast<RepeatedProtoItem*>(item);
    if(rItem != nullptr)
    {
        rItem->addItem();
        beginInsertRows(index, rowCount(index), rowCount(index));
        endInsertRows();
    }
}

void ProtobufModel::onRemoveItem(const QModelIndex &index)
{
    if(index.parent().isValid())
    {
        int row = static_cast<ProtoTreeItem*>(index.internalPointer())->row();
        ProtoTreeItem *pItem = static_cast<ProtoTreeItem*>(index.parent().internalPointer());
        beginRemoveRows(index.parent(), row, row);
        pItem->removeRow(row);
        endRemoveRows();
    }
}

QModelIndex ProtobufModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!mRootItem || !hasIndex(row,column,parent))
        return QModelIndex();

    ProtoTreeItem *parentItem = parent.isValid() ?
        static_cast<ProtoTreeItem*>(parent.internalPointer()) : mRootItem.get();

    ProtoTreeItem *childItem = parentItem->child(row);
    return childItem != nullptr ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex ProtobufModel::parent(const QModelIndex &index) const
{       
    if (!mRootItem || !index.isValid())
        return QModelIndex();

    ProtoTreeItem *childItem = static_cast<ProtoTreeItem*>(index.internalPointer());
    ProtoTreeItem *parentItem = childItem->parentItem();

    return parentItem == mRootItem.get() ?
        QModelIndex() : createIndex(parentItem->row(), 0, parentItem);
}

int ProtobufModel::rowCount(const QModelIndex &parent) const
{
    if (!mRootItem || parent.column() > 0)
        return 0;

    ProtoTreeItem *parentItem = parent.isValid() ?
        static_cast<ProtoTreeItem*>(parent.internalPointer()) : mRootItem.get();

    return parentItem->rowCount();
}

int ProtobufModel::columnCount(const QModelIndex &parent) const
{
    return COLUMN_COUNT;
}

QVariant ProtobufModel::data(const QModelIndex &index, int role) const
{
    if (!mRootItem || !index.isValid())
        return QVariant();

    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());

    switch (role)
    {
        case Qt::DisplayRole:
            return item->data(index.column());
        case Qt::BackgroundRole:
            return index.column() > 0 ? item->color() : QVariant();
        case Qt::DecorationRole:
            return index.column() == 0 ? icon(item) : QVariant();

    }
    return QVariant();
}

QIcon ProtobufModel::icon(ProtoTreeItem* item) const
{
    switch (item->label())
    {
        case proto::FieldDescriptor::LABEL_OPTIONAL:
            return QIcon(":/icons/optional.png");
        case proto::FieldDescriptor::LABEL_REPEATED:
            return QIcon(":/icons/repeated.png");
        case proto::FieldDescriptor::LABEL_REQUIRED:
            return QIcon(":/icons/required.png");
    }
    return QIcon();
}

QVariant ProtobufModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return "";
            case 1:
                return "Поле";
            case 2:
                return "Тип данных";
            case 3:
                return "Значение";
        }
    }
    return QVariant();
}


bool ProtobufModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole && index.column() == DATA_COLUMN)
    {
        ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());
        item->setData(value);
        return true;
    }
    return false;
}


Qt::ItemFlags ProtobufModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if(index.column() == DATA_COLUMN &&
        static_cast<ProtoTreeItem*>(index.internalPointer())->getDelegate() != nullptr)
        flags = flags | Qt::ItemIsEditable;
    return flags;
}
