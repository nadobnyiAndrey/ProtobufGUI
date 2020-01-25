#include "bytestprotoitem.h"

#include <QLineEdit>
#include <QMessageBox>

BytesProtoItem::BytesProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem), StringProtoItem(field, parentItem), MessageProtoItem(field, parentItem)
{

}

void BytesProtoItem::setDesc(const google::protobuf::Descriptor *desc)
{
    if(desc == nullptr)
        return;

    if(!value().isNull())
    {
        std::string subMessage = value().toString().toStdString();

        proto::DynamicMessageFactory f;
        proto::Message * m = f.GetPrototype(desc)->New();
        if(!m->ParseFromString(subMessage))
        {
            QMessageBox::critical(nullptr, "error", "failed parse message");
            return;
        }

        ProtoTreeItem::setDesc(desc);
        std::cout << "MESSAGE:\n" << m->Utf8DebugString() << std::endl;
        MessageProtoItem::initFieldValue(m);
        setValue(QVariant());
    }
    else
    {
        ProtoTreeItem::setDesc(desc);
    }
}

QItemDelegate *BytesProtoItem::getDelegate() const
{
    return descriptor() == nullptr ? StringProtoItem::getDelegate() : nullptr;
}

void BytesProtoItem::clearValue()
{
    setDesc(nullptr);
    ProtoTreeItem::clearValue();
}

void BytesProtoItem::setFieldValue(google::protobuf::Message * m)
{
    if(descriptor() == nullptr) {
        StringProtoItem::setFieldValue(m);
    } else {
        m->GetReflection()->SetString(m, field(), getMessage()->SerializeAsString());
    }
}

void BytesProtoItem::addFieldValue(google::protobuf::Message *m , const google::protobuf::FieldDescriptor * d)
{
    if(descriptor() == nullptr) {
        StringProtoItem::addFieldValue(m, d);
    } else {
        m->GetReflection()->AddString(m, field(), getMessage()->SerializeAsString());
    }
}

void BytesProtoItem::initFieldValue(const google::protobuf::Message * m)
{
    setDesc(nullptr);
    StringProtoItem::initFieldValue(m);
}

void BytesProtoItem::initRepeatedFieldValue(const google::protobuf::Message * m, int idx)
{
    setDesc(nullptr);
    StringProtoItem::initRepeatedFieldValue(m, idx);
}

QBrush BytesProtoItem::color() const
{
    return StringProtoItem::color();
}

