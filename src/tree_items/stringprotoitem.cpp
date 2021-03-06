#include "stringprotoitem.h"

#include <QLineEdit>

StringProtoItem::ItemDelegate::ItemDelegate(QObject *parent)
    : QItemDelegate(parent) {}

QWidget *StringProtoItem::ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    return new QLineEdit(parent);
}

void StringProtoItem::ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit * e = static_cast<QLineEdit*>(editor);
    e->setText(index.model()->data(index).toString());
}

void StringProtoItem::ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit * e = static_cast<QLineEdit*>(editor);
    model->setData(index, e->text());
}

StringProtoItem::StringProtoItem(ProtobufModel& model, const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(model, field, parentItem)
    , mDelegate(new ItemDelegate) {}

StringProtoItem::~StringProtoItem()
{
    delete mDelegate;
}

QItemDelegate *StringProtoItem::getDelegate() const
{
    return mDelegate;
}

void StringProtoItem::fillFieldValue(google::protobuf::Message *message)
{
    std::string sData = value().toString().toStdString();
    if(sData.length() > 0)
        message->GetReflection()->SetString(message, field(), sData);
}


void StringProtoItem::addFieldValue(google::protobuf::Message * message, const google::protobuf::FieldDescriptor * desc)
{
    std::string sData = value().toString().toStdString();
    if(sData.length() > 0)
        message->GetReflection()->AddString(message, desc, sData);
}


void StringProtoItem::initFieldValue(const google::protobuf::Message * message)
{
    setValue(QString::fromStdString(message->GetReflection()->GetString(*message, field())));
}


void StringProtoItem::initRepeatedFieldValue(const google::protobuf::Message * message, int idx)
{
    setValue(QString::fromStdString(message->GetReflection()->GetRepeatedString(*message, field(), idx)));
}
