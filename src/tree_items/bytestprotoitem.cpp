#include "bytestprotoitem.h"

#include <QLineEdit>
#include <QMessageBox>

#include "src/prototreeerror.h"

BytesProtoItem::BytesProtoItem(const proto::FieldDescriptor * field, ProtoTreeItem *parentItem)
    : ProtoTreeItem(field, parentItem), StringProtoItem(field, parentItem), MessageProtoItem(field, parentItem)
{

}

void BytesProtoItem::setDesc(const google::protobuf::Descriptor *desc)
{
    if(desc == nullptr)
        return;

    if(!value().isNull()) {
        std::string subMessage = value().toByteArray().toStdString();
        std::cout << "submessage\n";
        printHex(subMessage);

        proto::DynamicMessageFactory f;
        proto::Message * m = f.GetPrototype(desc)->New();
        if(!m->ParseFromString(subMessage))
            throw ProtoTreeError(m);

        ProtoTreeItem::setDesc(desc);
        for(auto& child: childItems())
            child->initFieldValue(m);
        setValue(QVariant());
    } else {
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

void BytesProtoItem::fillFieldValue(google::protobuf::Message * m)
{
    if(descriptor() == nullptr) {
        StringProtoItem::fillFieldValue(m);
    } else {
        auto subMessage = getMessage();
        m->GetReflection()->SetString(m, field(), subMessage->SerializeAsString());
        printHex(subMessage);
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
    setValue(QByteArray::fromStdString(m->GetReflection()->GetString(*m, field())));
    printHex(m->GetReflection()->GetString(*m, field()));            
}

void BytesProtoItem::initRepeatedFieldValue(const google::protobuf::Message * m, int idx)
{
    setDesc(nullptr);
    setValue(QString::fromStdString(m->GetReflection()->GetRepeatedString(*m, field(), idx)));
    printHex(m->GetReflection()->GetString(*m, field()));
}

QBrush BytesProtoItem::color() const
{
    return StringProtoItem::color();
}
