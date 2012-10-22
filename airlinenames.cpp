/*
 *  Copyright (C) 2010 Cutehacks AS. All rights reserved.
 *  info@cutehacks.com
 *  http://cutehacks.com
 *
 */

#include "airlinenames.h"
#include "node.h"
#include <QStack>

AirlineNames::AirlineNames(QObject *parent)
    : XmlDataSource(parent)
{
    setUrl(QUrl("http://flydata.avinor.no/airlineNames.asp"));
    setRefreshRate(86400000); //msecs == 24h
    QObject::connect(this, SIGNAL(dataUpdated(Node*)), this, SLOT(createMapping(Node*)));
}

AirlineNames::~AirlineNames()
{
}

QString AirlineNames::airlineName(const QString &code) const
{
    return m_mapping.value(code, code);
}

void AirlineNames::createMapping(Node *root)
{
    m_mapping.clear();
    QStack<Node*> pending;
    pending.push(root);
    while (!pending.isEmpty()) {
        Node *node = pending.pop();
        if (node->type() == "airlineName")
            m_mapping.insert(node->attribute("code"), node->attribute("name"));
        for (int i = 0; i < node->childCount(); ++i)
            pending.push(node->child(i));
    }
}
