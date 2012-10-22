/*
 *  Copyright (C) 2012 Cutehacks AS. All rights reserved.
 *  info@cutehacks.com
 *  http://cutehacks.com
 *
 *  This file is part of Fly.
 *
 *  Fly is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Fly is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Fly.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QHash>
#include <QList>
#include <QStack>
#include <QSharedData>
#include <QSharedDataPointer>
#include <QDebug>

class Node
{
    friend QDebug operator<<(QDebug dbg, const Node &n);
public:
    Node(Node *parent = 0) : m_parent(parent) { appendToParent(); }
    //Node(const Node &other) : parent(0) { d = other.d; }
    Node(const QString &type, Node *parent = 0) : m_parent(parent), m_type(type) { appendToParent(); }
    ~Node() { foreach (Node *child, m_children) delete child; }

    Node *parent() { return m_parent; }
    Node *child(int i) const { return m_children.at(i); }
    int childCount() const { return m_children.count(); }
    int indexOfChild(Node *child) const { return m_children.indexOf(child); }
    bool hasSiblings() const { return m_parent && (m_parent->childCount() > 1); }

    Node *firstChildOfType(const QString &type) const
    {
        foreach(Node *node, m_children)
            if (node->type() == type)
                return node;
        return 0;
    }

    QList<Node*> decendantsOfType(const QString &type)
    {
        QStack<Node*> hierarchy;
        hierarchy.push(this);
        QList<Node*> flat;
        while (!hierarchy.isEmpty()) {
            Node *current = hierarchy.pop();
            for (int i = 0; i < current->childCount(); ++i)
                hierarchy.push(current->child(i));
            if (current->type() == type)
                flat.prepend(current);
        }
        return flat;
    }

    void setType(const QString &type) { m_type = type; }
    QString type() const { return m_type; }

    void setText(const QString &text) { m_text = text; }
    QString text() const { return m_text; }

    void setAttribute(const QString &name, const QString &value) { m_attributes.insert(name, value); }
    QString attribute(const QString &name) const { return m_attributes.value(name); }

    QHash<QString,QString> attributes() const { return m_attributes; }

private:
    void appendChild(Node *child) { m_children.append(child); }
    void appendToParent() { if (m_parent) m_parent->appendChild(this); }

    Node *m_parent;
    QList<Node*> m_children;

    QString m_type;
    QString m_text;
    QHash<QString, QString> m_attributes;
};

inline QDebug operator<<(QDebug dbg, const Node &n)
{
    dbg << "<";
    dbg.nospace()<< n.m_type;
    if (!n.m_attributes.isEmpty())
        dbg.nospace() << ", " << n.m_attributes;
    if (!n.m_text.isEmpty())
        dbg.nospace() << ", " << n.m_text;
    dbg.space();
    if (!n.m_children.isEmpty()) {
        dbg.nospace() << endl;
        foreach (Node *child, n.m_children)
            dbg.nospace() << (*child) << endl;
    }
    dbg.nospace() << ">";
    return dbg;
}

#endif
